/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:36:07 by alonso            #+#    #+#             */
/*   Updated: 2024/09/27 12:54:22 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

void	ft_exit(char *args, t_minish *mini)
{
	t_exitState	state;
	int 		num;

	mini->exec = 1;
	num = -999999999;
	initialize_exit_state(&state, args);
	handle_empty_args(&state);
	state.num_args = count_args(state.args);
	handle_too_many_args(&state);
	if (state.num_args == 1)
	{
		process_exit_arg(&state);
		if (state.exit_value != num)
		{
			exit(state.exit_value);
		}
	}
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
