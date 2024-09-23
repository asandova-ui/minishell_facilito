/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:36:23 by alonso            #+#    #+#             */
/*   Updated: 2024/09/23 13:37:16 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

int	ft_export(char *args, t_minish *mini)
{
	char	*arg;
	char	*equal_sign;
	char	*name;
	char	*value;
	int		ret_value;

	ret_value = 0;
	if (args == NULL || *args == '\0')
	{
		return (print_sorted_envp(mini));
	}
	arg = strtok(args, " \n");
	while (arg != NULL)
	{
		equal_sign = strchr(arg, '=');
		if (equal_sign != NULL)
		{
			*equal_sign = '\0';
			name = arg;
			value = equal_sign + 1;
			if (is_valid_env(name))
			{
				if (add_or_update_env_var(mini, name, value) != 0)
					ret_value = 1;
			}
			else
			{
				fprintf(stderr, "export: %s: not a valid identifier\n", name);
				ret_value = 1;
			}
		}
		else if (!is_valid_env(arg))
		{
			fprintf(stderr, "export: %s: not a valid identifier\n", arg);
			ret_value = 1;
		}
		arg = strtok(NULL, " \n");
	}
	return (ret_value);
}

int	add_or_update_env_var(t_minish *mini, const char *name, const char *value)
{
	int		i;
	int		name_len;
	char	**new_envp;

	if (!mini || !mini->envp || !name || !value)
		return (1);
	i = 0;
	name_len = ft_strlen(name);
	while (mini->envp && mini->envp[i])
	{
		if (ft_strncmp(mini->envp[i], name, name_len) == 0
			&& mini->envp[i][name_len] == '=')
		{
			free(mini->envp[i]);
			mini->envp[i] = malloc(name_len + ft_strlen(value) + 2);
			if (!mini->envp[i])
				return (1);
			sprintf(mini->envp[i], "%s=%s", name, value);
			return (0);
		}
		i++;
	}
	new_envp = realloc(mini->envp, (i + 2) * sizeof(char *));
	if (!new_envp)
		return (1);
	mini->envp = new_envp;
	mini->envp[i] = malloc(name_len + ft_strlen(value) + 2);
	if (!mini->envp[i])
		return (1);
	sprintf(mini->envp[i], "%s=%s", name, value);
	mini->envp[i + 1] = NULL;
	return (0);
}

int	is_valid_env(const char *env)
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

int	print_error(int error, const char *arg)
{
	if (error == -1)
		ft_putstr_fd("export: not valid in this context: ", STDERR);
	else if (error == 0 || error == -3)
		ft_putstr_fd("export: not a valid identifier: ", STDERR);
	write(STDERR, arg, strlen(arg));
	write(STDERR, "\n", 1);
	return (1);
}

int	print_sorted_envp(t_minish *mini)
{
	char	**sorted_envp;
	int		i;
	int		env_count;

	if (!mini || !mini->envp)
		return (1);
	env_count = 0;
	while (mini->envp[env_count])
		env_count++;
	sorted_envp = malloc((env_count + 1) * sizeof(char *));
	if (!sorted_envp)
		return (1);
	i = 0;
	while (i < env_count)
	{
		sorted_envp[i] = mini->envp[i];
		i++;
	}
	sorted_envp[env_count] = NULL;
	ft_qsort(sorted_envp, env_count, sizeof(char *), compare_env);
	i = 0;
	while (i < env_count)
	{
		printf("declare -x %s\n", sorted_envp[i]);
		i++;
	}
	free(sorted_envp);
	return (0);
}

int	compare_env(const void *a, const void *b)
{
	return (ft_strcmp(*(const char **)a, *(const char **)b));
}

void	ft_qsort(void *base, size_t nitems, size_t size,
		int (*compar)(const void *, const void *))
{
	char	*pivot;
	void	*tmp;

	size_t i, j;
	if (nitems < 2)
		return ;
	pivot = base + (nitems / 2) * size;
	i = 0;
	j = nitems - 1;
	while (1)
	{
		while (compar(base + i * size, pivot) < 0)
			i++;
		while (compar(base + j * size, pivot) > 0)
			j--;
		if (i >= j)
			break ;
		tmp = malloc(size);
		if (!tmp)
			return ;
		ft_memcpy(tmp, base + i * size, size);
		ft_memcpy(base + i * size, base + j * size, size);
		ft_memcpy(base + j * size, tmp, size);
		free(tmp);
		if (pivot == base + i * size)
			pivot = base + j * size;
		else if (pivot == base + j * size)
			pivot = base + i * size;
		i++;
		j--;
	}
	ft_qsort(base, i, size, compar);
	ft_qsort(base + (i * size), nitems - i, size, compar);
}
