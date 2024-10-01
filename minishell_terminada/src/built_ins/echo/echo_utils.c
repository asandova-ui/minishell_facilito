/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:43:58 by jamorale          #+#    #+#             */
/*   Updated: 2024/10/01 09:40:04 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

int	handle_empty_arg(t_echoState *state)
{
	if (state->arg == NULL)
	{
		write(1, "\n", 1);
		free(state->processed_line);
		return (0);
	}
	return (1);
}

void	handle_n_flag(t_echoState *state)
{
	if (ft_strcmp(state->arg, "-n") == 0)
	{
		state->new_line = 0;
		state->arg = ft_strtok(NULL, " \n");
	}
}

void	process_variable(t_echoQuoteState *qstate, t_echoState *estate)
{
	char	*var_start;
	char	*var_end;
	size_t	var_len;
	char	*var_name;
	char	*env_value;

	var_start = qstate->current + 1;
	if (*var_start == '?')
	{
		ft_printf("%d", estate->mini->ret_value);
		qstate->current = var_start + 1;
		return ;
	}
	var_end = var_start;
	while (*var_end && (*var_end == '_' || ft_isalnum(*var_end)))
		var_end++;
	var_len = var_end - var_start;
	var_name = ft_strndup(var_start, var_len);
	env_value = mini_getenv(estate->mini, var_name);
	free(var_name);
	if (env_value)
	{
		write(1, env_value, ft_strlen(env_value));
	}
	qstate->current = var_end;
}

void	process_current_char(t_echoQuoteState *qstate)
{
	if (*qstate->current == '\'')
	{
		qstate->in_single_quote = !qstate->in_single_quote;
		write(1, qstate->current, 1);
		qstate->current++;
	}
	else if (*qstate->current == '\"')
	{
		qstate->in_double_quote = !qstate->in_double_quote;
		qstate->current++;
	}
	else
	{
		write(1, qstate->current, 1);
		qstate->current++;
	}
}

void	process_arg(t_echoState *estate)
{
	t_echoQuoteState	qstate;

	qstate.current = estate->arg;
	qstate.in_single_quote = 0;
	qstate.in_double_quote = 0;
	while (*qstate.current)
	{
		if (*qstate.current == '\'' && !qstate.in_double_quote)
		{
			qstate.in_single_quote = !qstate.in_single_quote;
			qstate.current++;
			continue ;
		}
		else if (*qstate.current == '\"' && !qstate.in_single_quote)
		{
			qstate.in_double_quote = !qstate.in_double_quote;
			qstate.current++;
			continue ;
		}
		else if (*qstate.current == '$' && (qstate.in_double_quote
				|| !qstate.in_single_quote))
			process_variable(&qstate, estate);
		else
			process_current_char(&qstate);
	}
}
