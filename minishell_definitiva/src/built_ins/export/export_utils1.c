/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 00:45:20 by jamorale          #+#    #+#             */
/*   Updated: 2024/09/24 01:08:50 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

void	init_export_sorted_env_state(SortedEnvState *state, t_minish *mini)
{
	state->env_count = 0;
	while (mini->envp[state->env_count])
		state->env_count++;
	state->sorted_envp = malloc((state->env_count + 1) * sizeof(char *));
}

void	copy_and_sort_export_env(SortedEnvState *state, t_minish *mini)
{
	int	i;

	i = 0;
	while (i < state->env_count)
	{
		state->sorted_envp[i] = mini->envp[i];
		i++;
	}
	state->sorted_envp[state->env_count] = NULL;
	ft_qsort(state->sorted_envp, state->env_count, sizeof(char *),
		compare_export_env);
}

int	print_export_sorted_envp(t_minish *mini)
{
	SortedEnvState	state;
	int				i;

	if (!mini || !mini->envp)
		return (1);
	init_export_sorted_env_state(&state, mini);
	if (!state.sorted_envp)
		return (1);
	copy_and_sort_export_env(&state, mini);
	i = 0;
	while (i < state.env_count)
	{
		printf("declare -x %s\n", state.sorted_envp[i]);
		i++;
	}
	free(state.sorted_envp);
	return (0);
}

int	compare_export_env(const void *a, const void *b)
{
	return (ft_strcmp(*(const char **)a, *(const char **)b));
}
