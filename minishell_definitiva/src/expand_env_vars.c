/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 12:42:28 by alonso            #+#    #+#             */
/*   Updated: 2024/09/26 16:33:03 by alonso           ###   ########.fr       */
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
	char		*result;
	size_t		prefix_len;
	size_t		suffix_len;
	size_t		new_len;
	const char	*replacement;

	prefix_len = start - str;
	suffix_len = strlen(start + env_var->var_len + 1);
	env_var->value = get_env_value(env_var->name, mini);
	// printf("VALOR======  %s\n", env_var->value);
	replacement = env_var->value ? env_var->value : "";
	new_len = prefix_len + strlen(replacement) + suffix_len + 1;
	result = malloc(new_len);
	// printf("VALOR======  %s\n", env_var->value);
	if (result)
	{
		strncpy(result, str, prefix_len);
		strcpy(result + prefix_len, replacement);
		strcpy(result + prefix_len + strlen(replacement), start
			+ env_var->var_len + 1);
	}
	if (env_var->value)
		free(env_var->value);
	// printf("VALOR======  %s-\n", result);
	// printf("%s\n", result);
	return (result);
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

char	*expand_env_vars(char *str, t_minish *mini)
{
	t_env_var env_var;
	char *result;
	char *current;
	int in_single_quote = 0;
	int i, j;
	int should_expand = 1;

	init_env_var(&env_var);
	result = ft_strdup(str);
	if (!result)
		return (NULL);

	// Primero, revisamos si hay comillas simples
	for (i = 0; result[i]; i++)
	{
		if (result[i] == '\'')
		{
			in_single_quote = !in_single_quote;
			should_expand = 0;
			break ;
		}
	}

	// Si hay comillas simples, las eliminamos sin expandir
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

	// Si no hay comillas simples, procedemos con la expansión normal
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
}