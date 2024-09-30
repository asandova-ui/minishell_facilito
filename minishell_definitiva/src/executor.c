/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:06:54 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 21:05:58 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_command_context(t_command_context *ctx, char *line, t_minish *mini)
{
	ctx->line = line;
	ctx->trimmed_line = NULL;
	ctx->expanded_line = NULL;
	ctx->mini = mini;
	ctx->final_line = NULL;
}

void	handle_child_process(char **args, t_minish *mini, t_redirection *red)
{
	execute_command(args, mini, red);
	exit(EXIT_FAILURE);
}

void	handle_parent_process(pid_t pid, t_minish *mini)
{
	int	status;

	waitpid(pid, &status, 0);
	(void)mini;
	if (WIFEXITED(status))
		mini->ret_value = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->ret_value = 128 + WTERMSIG(status);
	else
		mini->ret_value = 1;
}

void	execute_single_command(t_command_context *ctx)
{
	t_redirection	red;
	char			**args;
	pid_t			pid;

	init_redirection(&red);
	args = parse_command(ctx->expanded_line, &red);
	pid = fork();
	if (pid == -1)
		ctx->mini->ret_value = 1;
	else if (pid == 0)
		handle_child_process(args, ctx->mini, &red);
	else
		handle_parent_process(pid, ctx->mini);
	free_redirection(&red);
	free_args(args);
}

void	run_command(char *line, t_minish *mini)
{
	t_command_context	ctx;
	char				*quoted_line;
	char				*second_line;

	quoted_line = handle_quotes(line);
	//printf("LINEA ORIGINAL1:    %s\n", quoted_line);
	init_command_context(&ctx, quoted_line, mini);
	//printf("LINEA ORIGINAL:     %s\n", ctx.line);
	second_line = reemplazar_comillas(ctx.line);
	//printf("LINEA ORIGINAL2:    %s\n", second_line);
	ctx.trimmed_line = trim_whitespace(second_line);
	ctx.trimmed_line = parse_command_quotes(ctx.trimmed_line);
	//printf("LINEA PARSED  :     %s\n", ctx.trimmed_line);
	ctx.final_line = expand_env_vars(ctx.trimmed_line, mini);
	//printf("LINEA EXPANDED   :  %s\n", ctx.final_line);
	ctx.expanded_line = morethings(ctx.final_line);
	//printf("LINEA FINAL   :     %s\n", ctx.expanded_line);
	if (ft_strchr(ctx.expanded_line, '|'))
		execute_pipeline(ctx.expanded_line, ctx.mini);
	else
		execute_single_command(&ctx);
	cleanup_command_context(&ctx);
	free(quoted_line);
	free(second_line);
	return ;
}
