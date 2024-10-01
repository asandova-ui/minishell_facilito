/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:34:43 by alonso            #+#    #+#             */
/*   Updated: 2024/10/01 09:08:40 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

int	ft_echo(char *line, t_minish *mini)
{
	t_echoState	state;

	initialize_echo_state(&state, line, mini);
	if (state.processed_line == NULL)
		return (1);
	state.arg = ft_strtok(state.processed_line, " \n");
	if (!handle_empty_arg(&state))
		return (0);
	handle_n_flag(&state);
	while (state.arg != NULL)
	{
		process_arg(&state);
		state.arg = ft_strtok(NULL, " \n");
		if (state.arg != NULL)
			write(1, " ", 1);
	}
	if (state.new_line)
		write(1, "\n", 1);
	free(state.processed_line);
	return (0);
}

char	*processs_quotes(t_quote_state *state, char *result)
{
	char	*extra_input;
	int		is_first_input;

	is_first_input = 1;
	while (state->in_single_quote || state->in_double_quote)
	{
		ft_printf("quote> ");
		extra_input = readline(NULL);
		if (!extra_input)
			break ;
		result = allocate_new_result(result, extra_input, is_first_input);
		append_input(&result, extra_input, is_first_input);
		is_first_input = 0;
		update_quote_state(extra_input, state);
		free(extra_input);
	}
	return (result);
}

char	*handle_quotes(const char *str)
{
	t_quote_state	state;
	char			*result;

	state.in_single_quote = 0;
	state.in_double_quote = 0;
	result = initialize_result(str);
	update_quote_state(str, &state);
	return (processs_quotes(&state, result));
}
