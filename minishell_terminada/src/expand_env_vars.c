/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 12:42:28 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 19:31:37 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*trim_whitespace(char *str)
{
	char	*end;

	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	end = str + ft_strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;
	*(end + 1) = '\0';
	return (str);
}

char	*find_next_env_var(char *start, t_env_var *env_var)
{
	char	*dollar;

	dollar = ft_strchr(start, '$');
	if (!dollar)
		return (NULL);
	env_var->var_len = 0;
	if (dollar[1] == '?')
	{
		env_var->var_len = 1;
		env_var->name = ft_strdup("?");
		return (dollar);
	}
	while (dollar[env_var->var_len + 1] && (isalnum(dollar[env_var->var_len
					+ 1]) || dollar[env_var->var_len + 1] == '_'))
	{
		env_var->var_len++;
	}
	if (env_var->var_len > 0)
	{
		env_var->name = ft_strndup(dollar + 1, env_var->var_len);
		return (dollar);
	}
	return (find_next_env_var(dollar + 1, env_var));
}

char	*process_env_vars(char *str, char *current, t_env_var *env_var,
		t_minish *mini)
{
	char	*result;
	char	*new_result;

	result = str;
	while (current != NULL)
	{
		new_result = replace_env_var(result, current, env_var, mini);
		if (new_result)
		{
			if (result != str)
				free(result);
			result = new_result;
			current = result + (current - str);
		}
		else
		{
			if (result != str)
				free(result);
			return (NULL);
		}
		free(env_var->name);
		env_var->name = NULL;
		current = find_next_env_var(current, env_var);
	}
	return (result);
}
