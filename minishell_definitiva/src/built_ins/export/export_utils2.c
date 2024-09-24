/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 00:52:51 by jamorale          #+#    #+#             */
/*   Updated: 2024/09/24 00:52:54 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

int	update_existing_export_env_var(EnvVarState *state, int i, int name_len)
{
	free(state->mini->envp[i]);
	state->mini->envp[i] = malloc(name_len + ft_strlen(state->value) + 2);
	if (!state->mini->envp[i])
		return (1);
	sprintf(state->mini->envp[i], "%s=%s", state->name, state->value);
	return (0);
}

int	add_new_export_env_var(EnvVarState *state, int i, int name_len)
{
	char	**new_envp;

	new_envp = realloc(state->mini->envp, (i + 2) * sizeof(char *));
	if (!new_envp)
		return (1);
	state->mini->envp = new_envp;
	state->mini->envp[i] = malloc(name_len + ft_strlen(state->value) + 2);
	if (!state->mini->envp[i])
		return (1);
	sprintf(state->mini->envp[i], "%s=%s", state->name, state->value);
	state->mini->envp[i + 1] = NULL;
	return (0);
}

int	add_or_update_export_env_var(t_minish *mini, const char *name,
		const char *value)
{
	EnvVarState	state;
	int			i;
	int			name_len;

	init_export_env_var_state(&state, mini, name, value);
	if (!state.mini || !state.mini->envp || !state.name || !state.value)
		return (1);
	i = 0;
	name_len = ft_strlen(state.name);
	while (state.mini->envp && state.mini->envp[i])
	{
		if (ft_strncmp(state.mini->envp[i], state.name, name_len) == 0
			&& state.mini->envp[i][name_len] == '=')
		{
			return (update_existing_export_env_var(&state, i, name_len));
		}
		i++;
	}
	return (add_new_export_env_var(&state, i, name_len));
}

int	is_valid_export_env(const char *env)
{
	int	i;

	i = 0;
	if (ft_isdigit(env[i]))
		return (0);
	while (env[i] && env[i] != '=')
	{
		if (!ft_isalnum(env[i]) && env[i] != '_')
			return (-1);
		i++;
	}
	if (env[i] != '=')
		return (2);
	return (1);
}

int	print_export_error(int error, const char *arg)
{
	if (error == -1)
		ft_putstr_fd("export: not valid in this context: ", STDERR);
	else if (error == 0 || error == -3)
		ft_putstr_fd("export: not a valid identifier: ", STDERR);
	write(STDERR, arg, strlen(arg));
	write(STDERR, "\n", 1);
	return (1);
}
