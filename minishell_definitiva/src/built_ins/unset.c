/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:37:53 by alonso            #+#    #+#             */
/*   Updated: 2024/09/24 03:17:10 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

int	ft_unset(char *args, t_minish *mini)
{
	char	*arg;
	int		ret_value;

	ret_value = 0;
	arg = strtok(args, " \n");
	while (arg != NULL)
	{
		if (is_valid_export_env(arg))
		{
			if (remove_env_var(mini, arg) != 0)
				ret_value = 1;
		}
		else
		{
			fprintf(stderr, "unset: %s: not a valid identifier\n", arg);
			ret_value = 1;
		}
		arg = strtok(NULL, " \n");
	}
	return (ret_value);
}

int	remove_env_var(t_minish *mini, const char *name)
{
	int	i;
	int	j;
	int	name_len;

	if (!mini || !mini->envp || !name)
		return (1);
	i = 0;
	name_len = ft_strlen(name);
	while (mini->envp[i])
	{
		if (ft_strncmp(mini->envp[i], name, name_len) == 0
			&& mini->envp[i][name_len] == '=')
		{
			free(mini->envp[i]);
			j = i;
			while (mini->envp[j])
			{
				mini->envp[j] = mini->envp[j + 1];
				j++;
			}
			return (0);
		}
		i++;
	}
	return (1);
}
