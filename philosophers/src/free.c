/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 13:14:41 by marvin            #+#    #+#             */
/*   Updated: 2024/05/06 13:14:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	free_philos(t_program *program, int amound)
{
	int	index;

	index = -1;
	if (program->philos != NULL)
	{
		while (++index < amound)
			if (program->philos[index].ptid != NULL)
				free((program->philos[index]).ptid);
		free(program->philos);
	}
}

void	f_p(t_program *program)
{
	if (program->tid != NULL)
		free(program->tid);
	free_philos(program, program->philo_num);
	if (program->forks != NULL)
		free(program->forks);
	free(program);
}
