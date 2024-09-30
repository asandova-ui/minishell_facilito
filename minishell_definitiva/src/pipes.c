/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 10:08:30 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 10:45:34 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_pipes(t_pipeline *pipeline)
{
	int	i;

	i = 0;
	while (i < pipeline->cmd_count - 1)
	{
		if (pipe(pipeline->pipes[i]) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	child_process(t_pipeline *pipeline, t_minish *mini, int i)
{
	t_redirection	red;
	char			*trimmed_cmd;
	char			**args;

	if (i > 0)
		dup2(pipeline->pipes[i - 1][0], STDIN_FILENO);
	if (i < pipeline->cmd_count - 1)
		dup2(pipeline->pipes[i][1], STDOUT_FILENO);
	close_all_pipes(pipeline);
	init_redirection(&red);
	trimmed_cmd = trim_whitespace(pipeline->commands[i]);
	args = parse_command(trimmed_cmd, &red);
	execute_command(args, mini, &red);
	exit(EXIT_FAILURE);
}

void	parent_process(t_pipeline *pipeline, t_minish *mini)
{
	int	status;
	int	i;

	i = 0;
	close_all_pipes(pipeline);
	while (i < pipeline->cmd_count)
	{
		waitpid(pipeline->pids[i], &status, 0);
		if (i == pipeline->cmd_count - 1)
		{
			if (WIFEXITED(status))
				mini->ret_value = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				mini->ret_value = 128 + WTERMSIG(status);
			else
				mini->ret_value = 1;
		}
		i++;
	}
}

void	cleanup_pipeline(t_pipeline *pipeline)
{
	int	i;

	i = 0;
	while (pipeline->commands[i])
	{
		free(pipeline->commands[i]);
		i++;
	}
	free(pipeline->commands);
}

void	execute_pipeline(char *line, t_minish *mini)
{
	t_pipeline	pipeline;
	int			i;

	i = -1;
	pipeline.commands = ft_split(line, '|');
	pipeline.cmd_count = 0;
	while (pipeline.commands[pipeline.cmd_count])
		pipeline.cmd_count++;
	setup_pipes(&pipeline);
	while (++i < pipeline.cmd_count)
	{
		pipeline.pids[i] = fork();
		if (pipeline.pids[i] == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pipeline.pids[i] == 0)
			child_process(&pipeline, mini, i);
	}
	parent_process(&pipeline, mini);
	cleanup_pipeline(&pipeline);
}
