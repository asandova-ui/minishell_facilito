/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 13:21:27 by marvin            #+#    #+#             */
/*   Updated: 2024/05/06 13:21:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	unlock_mutexes(int lf, int rf, int w, t_philo **philo)
{
	if (rf)
		pthread_mutex_unlock((*philo)->r_fork);
	if (lf)
		pthread_mutex_unlock((*philo)->l_fork);
	if (w)
		pthread_mutex_unlock((*philo)->write);
}

static int	if_waited_to_long(t_philo **philo)
{
	pthread_mutex_lock(&(*philo)->lock);
	if ((*philo)->status == -1)
	{
		pthread_mutex_unlock(&(*philo)->lock);
		return (1);
	}
	pthread_mutex_unlock(&(*philo)->lock);
	if (((get_current_time() - (*philo)->last_meal_at)) > (*philo)->time_to_die)
	{
		pthread_mutex_lock(&(*philo)->lock);
		(*philo)->status = -1;
		pthread_mutex_unlock(&(*philo)->lock);
		return (1);
	}
	return (0);
}

static int	is_even(t_philo **philo)
{
	pthread_mutex_lock((*philo)->r_fork);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(0, 1, 0, &(*philo)), 1);
	pthread_mutex_lock((*philo)->write);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(0, 1, 1, &(*philo)), 1);
	pthread_mutex_lock(&(*philo)->lock);
	(*philo)->status = 4;
	pthread_mutex_unlock(&(*philo)->lock);
	if (what_to_print((*philo)->status, &(*philo)))
		return (unlock_mutexes(0, 1, 1, &(*philo)), 1);
	pthread_mutex_unlock((*philo)->write);
	pthread_mutex_lock((*philo)->l_fork);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(1, 1, 0, &(*philo)), 1);
	if (eating(&(*philo)))
		return (1);
	return (0);
}

static int	is_odd(t_philo **philo)
{
	pthread_mutex_lock((*philo)->l_fork);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(1, 0, 0, &(*philo)), 1);
	pthread_mutex_lock((*philo)->write);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(1, 0, 1, &(*philo)), 1);
	pthread_mutex_lock(&(*philo)->lock);
	(*philo)->status = 4;
	pthread_mutex_unlock(&(*philo)->lock);
	if (what_to_print((*philo)->status, &(*philo)))
		return (unlock_mutexes(1, 0, 1, &(*philo)), 1);
	pthread_mutex_unlock((*philo)->write);
	if ((*philo)->r_fork == NULL)
		return (unlock_mutexes(1, 0, 0, &(*philo)), 1);
	pthread_mutex_lock((*philo)->r_fork);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(1, 1, 0, &(*philo)), 1);
	if (eating(&(*philo)))
		return (1);
	return (0);
}

int	is_eating(t_philo **philo)
{
	if (((*philo)->id % 2) == 0)
		return (is_even(&(*philo)));
	else
		return (is_odd(&(*philo)));
}
