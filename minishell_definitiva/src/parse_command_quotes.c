/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 20:12:22 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 10:25:19 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_single_quote(char c, t_parse_state *state, char next)
{
	state->in_single = !state->in_single;
	if (state->is_echo && next == '$')
		state->result[state->index++] = c;
	else if (!state->in_single)
		state->result[state->index++] = c;
}

static void	handle_double_quote(t_parse_state *state)
{
	state->in_double = !state->in_double;
	if (!state->is_echo)
		state->result[state->index++] = '"';
}

static void	process_char(char c, t_parse_state *state)
{
	if (c == ' ' && !state->in_single && !state->in_double)
	{
		if (!state->last_space)
		{
			state->result[state->index++] = c;
			state->last_space = true;
		}
	}
	else
	{
		state->result[state->index++] = c;
		state->last_space = false;
	}
}

void	initialize_state(t_parse_state *state, const char *command, int len)
{
	state->in_single = false;
	state->in_double = false;
	state->last_space = true;
	state->is_echo = is_echo_command(command);
	state->result = malloc(len + 1);
	state->index = 0;
}

char	*parse_command_quotes(const char *command)
{
	int				len;
	t_parse_state	state;
	int				i;

	if (command[0] == '\'' || command[0] == '"')
		return (strdup(command));
	len = strlen(command);
	initialize_state(&state, command, len);
	i = 0;
	while (i < len)
	{
		if (command[i] == '\'' && !state.in_double)
			handle_single_quote('\'', &state, command[i + 1]);
		else if (command[i] == '"' && !state.in_single)
			handle_double_quote(&state);
		else if (state.is_echo && state.in_double && command[i] == '$')
			state.result[state.index++] = '$';
		else
			process_char(command[i], &state);
		i ++;
	}
	if (state.index > 0 && state.result[state.index - 1] == ' ')
		state.index--;
	state.result[state.index] = '\0';
	return (state.result);
}
