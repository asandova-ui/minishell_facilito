/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:07:56 by marvin            #+#    #+#             */
/*   Updated: 2024/05/06 12:07:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	destroy_philo_mutex(t_philo *philo, int number)
{
	int	index;

	index = -1;
	while (++index < number)
		pthread_mutex_destroy(&philo[index].lock);
}

void	d_mut(t_program *program, int number, int extra)
{
	int	index;

	index = -1;
	while (++index < number)
		pthread_mutex_destroy(&program->forks[index]);
	if (extra >= 1)
		pthread_mutex_destroy(&program->lock);
	if (extra >= 2)
		pthread_mutex_destroy(&program->write);
}

int	custom_error(char *string, t_program *program)
{
	size_t	index;

	index = -1;
	while (string[++index] != '\0')
		write(2, &string[index], 1);
	if (program != NULL)
	{
		destroy_philo_mutex(program->philos, program->philo_num);
		d_mut(program, program->philo_num, 2);
		f_p(program);
	}
	return (1);
}
