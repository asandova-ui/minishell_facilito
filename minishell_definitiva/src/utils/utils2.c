/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 21:13:38 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 10:38:46 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i] != NULL)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	cleanup_command_context(t_command_context *ctx)
{
	free(ctx->expanded_line);
	if (ctx->trimmed_line != ctx->line)
		free(ctx->trimmed_line);
}

bool	is_echo_command(const char *command)
{
	return (strncmp(command, "/bin/echo ", 10) == 0);
}

void	add_to_history(t_history *history, char *command)
{
	int	i;
	int	solve;

	i = 1;
	solve = check_spaces_history(command);
	if (solve != 0)
	{
		add_history(command);
		if (history->count < 1000)
			history->history[history->count++] = strdup(command);
		else
		{
			free(history->history[0]);
			while (i < 1000)
			{
				history->history[i - 1] = history->history[i];
				i++;
			}
			history->history[999] = strdup(command);
		}
	}
}

void	init_struct(t_minish *mini, char **envp)
{
	mini->in = dup(STDIN_FILENO);
	mini->out = dup(STDOUT_FILENO);
	mini->exit = 0;
	mini->ret_value = 0;
	mini->envp = NULL;
	mini->env_exist = 0;
	mini->exec = 0;
	mini->pid = 0;
	if (envp)
		mini->envp = dup_envp(envp);
	else
		mini->envp = NULL;
}
