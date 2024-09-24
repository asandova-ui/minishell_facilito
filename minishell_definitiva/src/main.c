/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:48:45 by alonso            #+#    #+#             */
/*   Updated: 2024/09/24 02:26:30 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

t_minish *g_mini;

char	*get_prompt(int ret_value)
{
	char	*prompt;

	prompt = malloc(100);
	if (prompt == NULL)
	{
		perror("malloc");
		exit(1);
	}
	if (ret_value == 0)
	{
		snprintf(prompt, 100, "\033[1;32m→ minishell ▸ \033[0m");
	}
	else
	{
		snprintf(prompt, 100, "\033[1;31m→ minishell ▸ \033[0m");
	}
	return (prompt);
}

int	main(int argc, char **argv, char **envp)
{
	t_minish	mini;
	t_history	*history;

	(void)argc;
	(void)argv;
	init_struct(&mini, envp);
	g_mini = &mini;
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
		mini.exec = 0;
	}
	free_history(history);
	free_envp(mini.envp);
	exit(EXIT_SUCCESS);
}

int	minishell(t_minish *mini, t_history *history)
{
	char	*temp;
	char	*line;
	char	*prompt;

	init_path(mini);
	//sig_init();
	//prompt = get_prompt(mini->ret_value);
	prompt= "\033[1;32m→ minishell ▸ \033[0m";
	line = readline(prompt);
	//free(prompt);
    if (line == NULL)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "exit\n", 5);
		free(line);
        return 1;
    }
	if (line == NULL)
		return (1);
	add_to_history(history, line);
	temp = ft_strdup2(line);
	built_ins(temp, mini, history);
	if (mini->exec == 0)
		mini->ret_value = run_command(line, mini);
	free(temp);
	free(line);
	line = NULL;
	if (mini->path)
	{
		free_paths(mini->path);
		mini->path = NULL;
	}
	if (mini->redisplay_prompt)
    {
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        mini->redisplay_prompt = 0;
    }
	return (0);
}

void	add_to_history(t_history *history, char *command)
{
	int	i;

	i = 1;
	add_history(command);
	if (history->count < 1000)
	{
		history->history[history->count++] = strdup(command);
	}
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
	mini->redisplay_prompt = 0;
	if (envp)
		mini->envp = dup_envp(envp);
	else
		mini->envp = NULL;
}
