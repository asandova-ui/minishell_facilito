/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:06:54 by alonso            #+#    #+#             */
/*   Updated: 2024/09/23 12:06:55 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

void	init_command_context(t_command_context *ctx, char *line, t_minish *mini)
{
	ctx->line = line;
	ctx->trimmed_line = trim_whitespace(line);
	ctx->expanded_line = expand_env_vars(ctx->trimmed_line, mini);
	ctx->mini = mini;
}

void	cleanup_command_context(t_command_context *ctx)
{
	if (ctx->expanded_line)
		free(ctx->expanded_line);
	if (ctx->trimmed_line && ctx->trimmed_line != ctx->line)
		free(ctx->trimmed_line);
}

void	execute_single_command(t_command_context *ctx)
{
	t_redirection	red;
	char			**args;
	pid_t			pid;
	int				status;
	int				i;

	init_redirection(&red);
	args = parse_command(ctx->expanded_line, &red);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		ctx->mini->ret_value = 1;
	}
	else if (pid == 0)
	{
		execute_command(args, ctx->mini, &red);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			ctx->mini->ret_value = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			ctx->mini->ret_value = 128 + WTERMSIG(status);
		else
			ctx->mini->ret_value = 1;
	}
	free_redirection(&red);
	i = -1;
	while (args[++i])
		free(args[i]);
	free(args);
}

int	run_command(char *line, t_minish *mini)
{
	t_command_context	ctx;

	init_command_context(&ctx, line, mini);
	if (ft_strchr(ctx.expanded_line, '|') != NULL)
		execute_pipeline(ctx.expanded_line, ctx.mini);
	else
		execute_single_command(&ctx);
	cleanup_command_context(&ctx);
	return (mini->ret_value);
}
