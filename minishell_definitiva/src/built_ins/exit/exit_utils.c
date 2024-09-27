/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:55:09 by jamorale          #+#    #+#             */
/*   Updated: 2024/09/27 12:53:05 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

void	initialize_exit_state(t_exitState *state, char *args)
{
	state->args = args;
	state->num_args = 0;
	state->exit_value = -999999999;
}

void	handle_empty_args(t_exitState *state)
{
	if (state->args == NULL || *(state->args) == '\0')
	{
		exit(EXIT_SUCCESS);
	}
}

int	count_args(char *args)
{
	int		count;
	char	*temp_args;
	char	*arg;

	count = 0;
	temp_args = strdup(args);
	if (!temp_args)
	{
		perror("strdup");
		exit(1);
	}
	arg = strtok(temp_args, " \n");
	while (arg != NULL)
	{
		count++;
		arg = strtok(NULL, " \n");
	}
	free(temp_args);
	return (count);
}

void	handle_too_many_args(t_exitState *state)
{
	if (state->num_args > 1)
	{
		fprintf(stderr, "exit\nbash: exit: too many arguments\n");
	}
}

void	process_exit_arg(t_exitState *state)
{
	if (!is_valid_number(state->args))
	{
		fprintf(stderr, "exit\nbash: exit: %s: numeric argument required\n",
			state->args);
	}
	else
	{
		state->exit_value = ft_atoi(state->args);
	}
}
