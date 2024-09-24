/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:48:35 by alonso            #+#    #+#             */
/*   Updated: 2024/09/23 12:00:52 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

void	init_redirection(t_redirection *red)
{
	red->infile = NULL;
	red->outfile = NULL;
	red->heredoc_delim = NULL;
	red->append_mode = 0;
}

void	free_redirection(t_redirection *red)
{
	free(red->infile);
	free(red->outfile);
	free(red->heredoc_delim);
}

void	handle_redirection(char *token, char *file, t_redirection *red)
{
	if (ft_strcmp(token, "<") == 0)
	{
		free(red->infile);
		red->infile = ft_strdup(file);
	}
	else if (ft_strcmp(token, ">") == 0)
	{
		free(red->outfile);
		red->outfile = ft_strdup(file);
		red->append_mode = 0;
	}
	else if (ft_strcmp(token, ">>") == 0)
	{
		free(red->outfile);
		red->outfile = ft_strdup(file);
		red->append_mode = 1;
	}
	else if (ft_strcmp(token, "<<") == 0)
	{
		free(red->heredoc_delim);
		red->heredoc_delim = ft_strdup(file);
	}
}

void	add_argument(char ***args, int *arg_count, char *token)
{
	*args = realloc(*args, (*arg_count + 2) * sizeof(char *));
	(*args)[(*arg_count)++] = ft_strdup(token);
}

char	**parse_command(char *cmd, t_redirection *red)
{
	char	**args;
	int		arg_count;
	char	*token;
	char	*rest;
	char	*file;

	args = NULL;
	arg_count = 0;
	rest = cmd;
	while ((token = strtok_r(rest, " \t", &rest)))
	{
		if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0
			|| ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
		{
			file = strtok_r(NULL, " \t", &rest);
			if (file)
				handle_redirection(token, file, red);
		}
		else
			add_argument(&args, &arg_count, token);
	}
	if (args)
		args[arg_count] = NULL;
	return (args);
}
