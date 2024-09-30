/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 12:42:28 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 11:55:47 by alonso           ###   ########.fr       */
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

/*char	*expand_env_vars(char *str, t_minish *mini)
{
	t_env_var	env_var;
	char		*result;
	char		*current;
	int			in_single_quote = 0;
	int			i, j;
	int			should_expand = 1;

	init_env_var(&env_var);
	result = ft_strdup(str);
	if (!result)
		return (NULL);

	if (str[0] == '\'')
	{
		return (result);
	}
	for (i = 0; result[i]; i++)
	{
		if (result[i] == '\'')
		{
			in_single_quote = !in_single_quote;
			should_expand = 0;
			break ;
		}
	}
	if (!should_expand)
	{
		for (i = 0, j = 0; result[i]; i++)
		{
			if (result[i] != '\'')
			{
				result[j++] = result[i];
			}
		}
		result[j] = '\0';
		return (result);
	}
	current = find_next_env_var(result, &env_var);
	if (current)
	{
		char *processed = process_env_vars(result, current, &env_var, mini);
		if (processed)
		{
			free(result);
			result = processed;
		}
		else
		{
			free(result);
			return (NULL);
		}
	}

	return (result);
}*/