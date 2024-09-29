/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:48:45 by alonso            #+#    #+#             */
/*   Updated: 2024/09/29 19:58:19 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

extern t_signal_status	g_signal_info;

int	main(int argc, char **argv, char **envp)
{
	t_minish	mini;
	t_history	*history;

	(void)argc;
	(void)argv;
	init_struct(&mini, envp);
	history = malloc(sizeof(t_history));
	if (!history)
	{
		free_envp(mini.envp);
		exit(EXIT_FAILURE);
	}
	setup_signals();
	init_struct_history(history);
	while (mini.exit == 0)
	{
		if (minishell(&mini, history))
			break ;
	}
	free_history(history);
	free_envp(mini.envp);
	exit(mini.ret_value);
}

int	minishell(t_minish *mini, t_history *history)
{
	char	*temp;
	char	*line;
	char	*prompt;

	init_path(mini);
	prompt = "\033[1;32m→ minishell ▸ \033[0m";
	g_signal_info.is_executing = 0;
	line = readline(prompt);
	if (g_signal_info.signal_status != 0)
	{
		mini->ret_value = g_signal_info.signal_status;
		g_signal_info.signal_status = 0;
	}
	if (line == NULL)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "exit\n", 5);
		mini->ret_value = 0;
		return (1);
	}
	if (*line)
	{
		add_to_history(history, line);
		temp = ft_strdup2(line);
		g_signal_info.is_executing = 1;
		built_ins(temp, mini, history);
		if (mini->exec == 0)
			run_command(line, mini);
		g_signal_info.is_executing = 0;
		if (g_signal_info.signal_status != 0)
		{
			mini->ret_value = g_signal_info.signal_status;
			g_signal_info.signal_status = 0;
		}
		free(temp);
	}
	free(line);
	if (mini->path)
	{
		free_paths(mini->path);
		mini->path = NULL;
	}
	return (0);
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
