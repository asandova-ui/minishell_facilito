/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:48:45 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 21:43:32 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	cleanup_minishell(t_minish *mini)
{
	if (mini->path)
	{
		free_paths(mini->path);
		mini->path = NULL;
	}
}

void	process_input(char *line, t_minish *mini, t_history *history)
{
	char	*temp;

	add_to_history(history, line);
	temp = ft_strdup2(line);
	g_signal_info.is_executing = 1;
	built_ins(temp, mini, history);
	if (mini->exec == 0)
		run_command(line, mini);
	g_signal_info.is_executing = 0;
	handle_signal_status_remix(mini);
	free(temp);
}

int	handle_null_line(char *line, t_minish *mini)
{
	if (line == NULL)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "exit\n", 5);
		mini->ret_value = 0;
		return (1);
	}
	return (0);
}

int	minishell(t_minish *mini, t_history *history)
{
	char	*line;
	char	*prompt;

	prompt = "\033[1;32m→ minishell ▸ \033[0m";
	init_path(mini);
	g_signal_info.is_executing = 0;
	line = readline(prompt);
	handle_signal_status_remix(mini);
	if (handle_null_line(line, mini))
		return (1);
	if (*line)
		process_input(line, mini, history);
	free(line);
	cleanup_minishell(mini);
	return (0);
}
