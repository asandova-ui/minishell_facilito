/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:35:49 by alonso            #+#    #+#             */
/*   Updated: 2024/09/23 18:47:18 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

int	ft_env(t_minish *mini)
{
	int	i;

	i = 0;
	while (mini->envp && mini->envp[i])
	{
		if (printf("%s\n", mini->envp[i]) < 0)
			return (1);
		i++;
	}
	return (0);
}
