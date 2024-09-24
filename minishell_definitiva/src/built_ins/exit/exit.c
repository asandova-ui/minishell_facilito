/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:36:07 by alonso            #+#    #+#             */
/*   Updated: 2024/09/24 09:55:53 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

void	ft_exit(char *args)
{
	t_exitState	state;

	initialize_exit_state(&state, args);
	handle_empty_args(&state);
	state.num_args = count_args(state.args);
	handle_too_many_args(&state);
	process_exit_arg(&state);
	exit(state.exit_value);
}

int	is_valid_number(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}
