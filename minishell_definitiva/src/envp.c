/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 11:01:23 by alonso            #+#    #+#             */
/*   Updated: 2024/09/23 11:02:13 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

void	init_path(t_minish *mini)
{
	char	**paths;
	int		i;

	paths = NULL;
	i = 0;
	while (mini->envp[i])
	{
		if (!ft_strncmp(mini->envp[i], "PATH=", 5))
			paths = ft_split(mini->envp[i] + 5, ':');
		i++;
	}
	mini->path = paths;
	return ;
}

int	check_spaces_history(char *command)
{
	int	i;

	i = 0;
	while (command[i])
	{
		if (command[i] > 32)
			return (1);
		i++;
	}
	return (0);
}
