/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:35:49 by alonso            #+#    #+#             */
/*   Updated: 2024/10/01 09:50:07 by alonso           ###   ########.fr       */
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
		if (ft_printf("%s\n", mini->envp[i]) < 0)
			return (1);
		i++;
	}
	return (0);
}
