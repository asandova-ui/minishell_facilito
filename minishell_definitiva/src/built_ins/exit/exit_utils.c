/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:55:09 by jamorale          #+#    #+#             */
/*   Updated: 2024/09/23 19:01:24 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

void	initialize_exit_state(ExitState *state, char *args)
{
	state->args = args;
	state->num_args = 0;
	state->exit_value = EXIT_SUCCESS;
}

void	handle_empty_args(ExitState *state)
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

void	handle_too_many_args(ExitState *state)
{
	if (state->num_args > 1)
	{
		fprintf(stderr, "exit\nbash: exit: too many arguments\n");
		exit(1);
	}
}

void	process_exit_arg(ExitState *state)
{
	if (!is_valid_number(state->args))
	{
		fprintf(stderr, "exit\nbash: exit: %s: numeric argument required\n",
			state->args);
		exit(255);
	}
	else
	{
		state->exit_value = ft_atoi(state->args);
	}
}
