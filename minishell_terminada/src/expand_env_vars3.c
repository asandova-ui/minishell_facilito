/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:55:26 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 11:57:56 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_expansion(char *result, char *current, t_env_var *env_var,
		t_minish *mini)
{
	char	*processed;

	if (current)
	{
		processed = process_env_vars(result, current, env_var, mini);
		if (processed)
		{
			free(result);
			return (processed);
		}
		else
		{
			free(result);
			return (NULL);
		}
	}
	return (result);
}

char	*remove_single_quotes_and_return(char *result)
{
	int		i;
	int		j;
	char	*new_result;

	j = 0;
	i = 0;
	new_result = malloc(strlen(result) + 1);
	if (!new_result)
		return (NULL);
	while (result[i])
	{
		if (result[i] != '\'')
			new_result[j++] = result[i];
		i++;
	}
	new_result[j] = '\0';
	free(result);
	return (new_result);
}

int	should_remove_single_quotes(const char *result)
{
	int	i;

	i = 0;
	while (result[i])
	{
		if (result[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

char	*expand_env_vars(char *str, t_minish *mini)
{
	t_env_var	env_var;
	char		*result;
	char		*current;

	init_env_var(&env_var);
	result = ft_strdup(str);
	if (!result)
		return (NULL);
	if (should_remove_single_quotes(result))
		return (remove_single_quotes_and_return(result));
	current = find_next_env_var(result, &env_var);
	return (handle_expansion(result, current, &env_var, mini));
}
