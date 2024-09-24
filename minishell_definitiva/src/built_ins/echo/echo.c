/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:34:43 by alonso            #+#    #+#             */
/*   Updated: 2024/09/24 09:51:14 by alonso           ###   ########.fr       */
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
	state.arg = strtok(state.processed_line, " \n");
	if (!handle_empty_arg(&state))
		return (0);
	handle_n_flag(&state);
	while (state.arg != NULL)
	{
		process_arg(&state);
		state.arg = strtok(NULL, " \n");
		if (state.arg != NULL)
			write(1, " ", 1);
	}
	if (state.new_line)
		write(1, "\n", 1);
	free(state.processed_line);
	return (0);
}
