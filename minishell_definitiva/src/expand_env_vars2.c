/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 10:44:22 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 11:34:51 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_env_var_value(t_env_var *env_var, const char *replacement)
{
	if (env_var->value)
		free(env_var->value);
	if (strcmp(env_var->name, "?") == 0 && env_var->value != replacement)
		free((char *)replacement);
}

char	*build_result_string(const char *str, char *start, t_env_var *env_var,
		t_build_string *build_str)
{
	char	*result;
	size_t	new_len;

	new_len = build_str->prefix_len + strlen(build_str->replacement)
		+ build_str->suffix_len + 1;
	result = malloc(new_len);
	if (result)
	{
		strncpy(result, str, build_str->prefix_len);
		strcpy(result + build_str->prefix_len, build_str->replacement);
		strcpy(result + build_str->prefix_len + strlen(build_str->replacement),
			start + env_var->var_len + 1);
	}
	return (result);
}

const char	*get_replacement_value(t_env_var *env_var, t_minish *mini)
{
	if (strcmp(env_var->name, "?") == 0)
	{
		env_var->value = ft_itoa(mini->ret_value);
		return (env_var->value);
	}
	else
	{
		env_var->value = get_env_value(env_var->name, mini);
		if (env_var->value)
			return (env_var->value);
		else
			return ("");
	}
}

char	*replace_env_var(const char *str, char *start, t_env_var *env_var,
		t_minish *mini)
{
	t_build_string	build_str;

	build_str.prefix_len = start - str;
	build_str.suffix_len = strlen(start + env_var->var_len + 1);
	build_str.replacement = get_replacement_value(env_var, mini);
	build_str.result = build_result_string(str, start, env_var, &build_str);
	free_env_var_value(env_var, build_str.replacement);
	return (build_str.result);
}
