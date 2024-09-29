/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 20:12:22 by alonso            #+#    #+#             */
/*   Updated: 2024/09/29 20:19:57 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
static void	toggle_single_quote(char c, bool *in_single_quotes,
		bool is_echo_command, char *result, int *result_index)
{
	if (c == '\'')
	{
		*in_single_quotes = !*in_single_quotes;
		if (is_echo_command && *in_single_quotes)
			result[(*result_index)++] = c;
	}
	else if (!*in_single_quotes)
	{
		result[(*result_index)++] = c;
	}
}

static void	toggle_double_quote(char c, bool *in_double_quotes, char *result,
		int *result_index)
{
	if (c == '"')
	{
		*in_double_quotes = !*in_double_quotes;
	}
	else if (!*in_double_quotes)
	{
		result[(*result_index)++] = c;
	}
}

static void	add_character_or_space(char c, char *result, int *result_index,
		bool *last_char_was_space)
{
	if (c == ' ' && *last_char_was_space)
		return ;
	result[(*result_index)++] = c;
	*last_char_was_space = (c == ' ');
}

char	*parse_command_quotes(const char *command)
{
	int		len;
	char	*result;
	bool	in_single_quotes = false, in_double_quotes = false,
			last_char_was_space;
	bool	is_echo_command;
	int		result_index;
	char	c;

	len = strlen(command);
	result = malloc(len + 1);
	in_single_quotes = false, in_double_quotes = false,
		last_char_was_space = true;
	is_echo_command = (strncmp(command, "/bin/echo ", 10) == 0);
	result_index = 0;
	if (command[0] == '\'' || command[0] == '"')
		return (strdup(command));
	for (int i = 0; i < len; i++)
	{
		c = command[i];
		toggle_single_quote(c, &in_single_quotes, is_echo_command, result,
			&result_index);
		toggle_double_quote(c, &in_double_quotes, result, &result_index);
		add_character_or_space(c, result, &result_index, &last_char_was_space);
	}
	if (result_index > 0 && result[result_index - 1] == ' ')
		result_index--;
	result[result_index] = '\0';
	return (result);
}*/

char	*parse_command_quotes(const char *command)
{
	int		len;
	char	*result;
	int		result_index;
	bool	in_single_quotes;
	bool	in_double_quotes;
	bool	last_char_was_space;
	bool	is_echo_command;
	char	c;

	if (command[0] == '\'' || command[0] == '"')
	{
		return (strdup(command));
	}
	len = strlen(command);
	result = malloc(len + 1);
	result_index = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	last_char_was_space = true;
	is_echo_command = (strncmp(command, "/bin/echo ", 10) == 0);
	for (int i = 0; i < len; i++)
	{
		c = command[i];
		if (c == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			if (is_echo_command && command[i + 1] == '$')
			{
				result[result_index++] = c;
			}
			else if (!in_single_quotes)
			{
				result[result_index++] = c;
			}
		}
		else if (c == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			if (!in_double_quotes && is_echo_command && command[i + 1] == '$')
			{
				continue ;
			}
		}
		else if (c == ' ' && !in_single_quotes && !in_double_quotes)
		{
			if (!last_char_was_space)
			{
				result[result_index++] = c;
				last_char_was_space = true;
			}
		}
		else
		{
			result[result_index++] = c;
			last_char_was_space = false;
		}
	}
	if (result_index > 0 && result[result_index - 1] == ' ')
	{
		result_index--;
	}
	result[result_index] = '\0';
	return (result);
}
