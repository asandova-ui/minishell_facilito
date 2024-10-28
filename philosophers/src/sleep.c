/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 13:21:31 by marvin            #+#    #+#             */
/*   Updated: 2024/05/06 13:21:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_usleep(size_t miliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < miliseconds)
		usleep(500);
	return (0);
}

int	ft_usleep_sleep(size_t miliseconds, t_philo *philo)
{
	size_t	start;

	start = get_current_time();
	pthread_mutex_lock(&philo->lock);
	if (philo->status == -1)
	{
		pthread_mutex_unlock(&philo->lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->lock);
	while ((get_current_time() - start) < miliseconds)
	{
		usleep(500);
		if (((get_current_time() - philo->last_meal_at)) > philo->time_to_die)
			return (1);
	}
	return (0);
}

static void	unlock_lock(t_philo **philo)
{
	pthread_mutex_unlock(&((*philo)->lock));
}

int	is_sleeping(t_philo **philo)
{
	size_t	time;

	pthread_mutex_lock(&((*philo)->lock));
	if ((*philo)->status == -1)
		return (unlock_lock(&(*philo)), 1);
	if ((*philo)->status != -1)
		(*philo)->status = 2;
	pthread_mutex_unlock(&((*philo)->lock));
	time = get_current_time() - (*philo)->start_time;
	pthread_mutex_lock((*philo)->write);
	printf("%zu %d is sleeping\n", time, (*philo)->id);
	pthread_mutex_unlock((*philo)->write);
	if (ft_usleep_sleep((*philo)->time_to_sleep, (*philo)))
	{
		pthread_mutex_lock(&((*philo)->lock));
		(*philo)->status = -1;
		return (unlock_lock(&(*philo)), 1);
	}
	else
	{
		pthread_mutex_lock(&((*philo)->lock));
		if ((*philo)->status != -1)
			(*philo)->status = 5;
		return (unlock_lock(&(*philo)), 0);
	}
}
