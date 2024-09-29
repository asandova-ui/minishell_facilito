/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 10:49:10 by alonso            #+#    #+#             */
/*   Updated: 2024/09/29 20:26:05 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

int	execute_from_path(char **args, t_minish *mini)
{
	int		i;
	char	*exec_path;

	if (!mini->path)
	{
		fprintf(stderr, "PATH is not set\n");
		return (0);
	}
	i = 0;
	while (mini->path[i])
	{
		exec_path = ft_strjoin_3args(mini->path[i], '/', args[0]);
		if (access(exec_path, X_OK) == 0)
		{
			execve(exec_path, args, mini->envp);
			perror("execve");
			free(exec_path);
			exit(126);
		}
		free(exec_path);
		i++;
	}
	return (0);
}

void	execute_command(char **args, t_minish *mini, t_redirection *red)
{
	apply_redirections(red);
	if (args[0][0] == '/' || args[0][0] == '.')
	{
		if (access(args[0], X_OK) == 0)
		{
			execve(args[0], args, mini->envp);
			perror("execve");
			exit(126);
		}
		else
		{
			fprintf(stderr, "Command not found or not executable: %s\n",
				args[0]);
			exit(127);
		}
	}
	else
	{
		if (mini->path != NULL)
		{
			if (execute_from_path(args, mini) == 0)
			{
				fprintf(stderr, "Command not found: %s\n", args[0]);
				exit(127);
			}
		}
		else
		{
			fprintf(stderr, "PATH is not set\n");
			exit(127);
		}
	}
	exit(126);
}
