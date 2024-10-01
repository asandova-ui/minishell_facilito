/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:36:23 by alonso            #+#    #+#             */
/*   Updated: 2024/10/01 09:49:33 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

void	init_export_state(t_exportState *state, char *args, t_minish *mini)
{
	state->args = args;
	state->mini = mini;
	state->ret_value = 0;
}

int	handle_export_empty_args(t_exportState *state)
{
	if (state->args == NULL || *(state->args) == '\0')
	{
		return (print_export_sorted_envp(state->mini));
	}
	return (-1);
}

void	process_export_arg(t_exportState *state, char *arg, char *name)
{
	char	*equal_sign;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign != NULL)
	{
		*equal_sign = '\0';
		name = arg;
		value = equal_sign + 1;
		if (is_valid_export_env(name))
		{
			if (add_or_update_export_env_var(state->mini, name, value) != 0)
				state->ret_value = 0;
		}
		else
		{
			ft_printf("export: %s: not a valid identifier\n", name);
			state->ret_value = 1;
		}
	}
	else if (!is_valid_export_env(arg))
	{
		ft_printf("export: %s: not a valid identifier\n", arg);
		state->ret_value = 1;
	}
}

int	ft_export(char *args, t_minish *mini)
{
	t_exportState	state;
	char			*arg;
	char			*name;

	name = "a";
	init_export_state(&state, args, mini);
	if (handle_export_empty_args(&state) != -1)
		return (state.ret_value);
	arg = ft_strtok(args, " \n");
	while (arg != NULL)
	{
		process_export_arg(&state, arg, name);
		arg = ft_strtok(NULL, " \n");
	}
	return (state.ret_value);
}

void	init_export_env_var_state(t_envVarState *state, t_minish *mini,
		const char *name, const char *value)
{
	state->mini = mini;
	state->name = name;
	state->value = value;
}
