/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 12:42:28 by alonso            #+#    #+#             */
/*   Updated: 2024/09/23 11:43:46 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

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

char	*replace_env_var(const char *str, char *start, t_env_var *env_var,
		t_minish *mini)
{
	char	*result;
	size_t	prefix_len;
	size_t	suffix_len;
	size_t	new_len;

	result = NULL;
	env_var->value = get_env_value(env_var->name, mini);
	if (env_var->value)
	{
		prefix_len = start - str;
		suffix_len = strlen(start + env_var->var_len + 1);
		new_len = prefix_len + strlen(env_var->value) + suffix_len + 1;
		result = malloc(new_len);
		if (result)
		{
			strncpy(result, str, prefix_len);
			strcpy(result + prefix_len, env_var->value);
			strcpy(result + prefix_len + strlen(env_var->value), start
				+ env_var->var_len + 1);
		}
		free(env_var->value);
	}
	return (result);
}

void	init_env_var(t_env_var	*env_var)
{
	env_var->name = NULL;
	env_var->value = NULL;
	env_var->var_len = 0;
}

char	*expand_env_vars(char *str, t_minish *mini)
{
	t_env_var	env_var;
	char		*result;
	char		*current;
	char		*new_result;

	init_env_var(&env_var);
	result = str;
	current = find_next_env_var(result, &env_var);
	while (current != NULL)
	{
		new_result = replace_env_var(result, current, &env_var, mini);
		if (new_result)
		{
			if (result != str)
				free(result);
			result = new_result;
			current = result + (current - str);
		}
		free(env_var.name);
		env_var.name = NULL;
		if (!new_result)
			current++;
		current = find_next_env_var(current, &env_var);
	}
	if (result == str)
		return (ft_strdup(str));
	return (result);
}
