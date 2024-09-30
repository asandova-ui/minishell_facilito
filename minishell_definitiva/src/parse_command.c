/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:48:35 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 10:45:34 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	process_token(t_parse_data *data, t_redirection *red)
{
	if (ft_strcmp(data->token, "<") == 0 || ft_strcmp(data->token, ">") == 0
		|| ft_strcmp(data->token, ">>") == 0 || ft_strcmp(data->token,
			"<<") == 0)
	{
		data->file = strtok_r(NULL, " \t", &data->rest);
		if (data->file)
			handle_redirection(data->token, data->file, red);
	}
	else
		add_argument(&data->args, &data->arg_count, data->token);
}

char	**parse_command(char *cmd, t_redirection *red)
{
	t_parse_data	data;

	data.args = NULL;
	data.arg_count = 0;
	data.token = 0;
	data.rest = cmd;
	data.file = NULL;
	data.token = strtok_r(data.rest, " \t", &data.rest);
	while (data.token)
	{
		process_token(&data, red);
		data.token = strtok_r(NULL, " \t", &data.rest);
	}
	if (data.args)
		data.args[data.arg_count] = NULL;
	return (data.args);
}
