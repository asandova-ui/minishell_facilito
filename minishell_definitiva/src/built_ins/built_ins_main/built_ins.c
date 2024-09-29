/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:33:56 by alonso            #+#    #+#             */
/*   Updated: 2024/09/29 20:18:54 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

void	handle_echo(t_cmd_ctx *ctx, t_minish *mini)
{
	char	*nueva_linea;

	ctx->args = strtok(NULL, "\n");
	if (ctx->args == NULL)
	{
		write(1, "\n", 1);
		mini->ret_value = 0;
	}
	else if (has_redirection(ctx->args))
	{
		nueva_linea = redirect_echo(ctx->line_copy);
		if (nueva_linea != NULL)
		{
			run_command(nueva_linea, mini);
			free(nueva_linea);
		}
	}
	else
	{
		if (mini->comillas != 1)
			ft_echo(ctx->args, mini);
		mini->ret_value = 0;
	}
	mini->exec = 1;
}

void	handle_cd(t_cmd_ctx *ctx, t_minish *mini)
{
	char	*home;

	ctx->args = strtok(NULL, "\n");
	if (ctx->args == NULL)
	{
		home = getenv("HOME");
		mini->ret_value = ft_cd(home, mini);
	}
	else
	{
		mini->ret_value = ft_cd(ctx->args, mini);
	}
	mini->exec = 1;
}

void	handle_export(t_cmd_ctx *ctx, t_minish *mini)
{
	int	error;

	ctx->args = strtok(NULL, "\n");
	if (ctx->args == NULL)
	{
		mini->ret_value = ft_export(NULL, mini);
	}
	else
	{
		error = is_valid_export_env(ctx->args);
		if (ctx->args[0] == '=')
			error = -3;
		if (error != 1)
		{
			print_export_error(error, ctx->args);
			mini->ret_value = 1;
		}
		else
		{
			mini->ret_value = ft_export(ctx->args, mini);
		}
	}
	mini->exec = 1;
}

void	dispatch_command(t_cmd_ctx *ctx, t_minish *mini, t_history *history)
{
	if (ft_strcmp(ctx->command, "echo") == 0)
		handle_echo(ctx, mini);
	else if (ft_strcmp(ctx->command, "cd") == 0)
		handle_cd(ctx, mini);
	else if (ft_strcmp(ctx->command, "pwd") == 0)
		mini->ret_value = ft_pwd();
	else if (ft_strcmp(ctx->command, "export") == 0)
		handle_export(ctx, mini);
	else if (ft_strcmp(ctx->command, "unset") == 0)
		handle_unset(ctx, mini);
	else if (ft_strcmp(ctx->command, "env") == 0)
		mini->ret_value = ft_env(mini);
	else if (ft_strcmp(ctx->command, "exit") == 0)
		ft_exit(strtok(NULL, "\n"), mini);
	else if (ft_strcmp(ctx->command, "history") == 0)
		show_history(history, mini);
	else
	{
		mini->ret_value = 127;
		mini->exec = 0;
	}
}

void	built_ins(char *line, t_minish *mini, t_history *history)
{
	t_cmd_ctx	ctx;

	ctx.line = line;
	ctx.line_copy = strdup(line);
	if (!ctx.line_copy)
	{
		mini->ret_value = 1;
		mini->exec = 1;
		return ;
	}
	ctx.command = strtok(ctx.line, " \n");
	if (ctx.command == NULL)
	{
		mini->ret_value = 0;
		mini->exec = 1;
		free(ctx.line_copy);
		return ;
	}
	dispatch_command(&ctx, mini, history);
	free(ctx.line_copy);
}
