/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:41:43 by marvin            #+#    #+#             */
/*   Updated: 2024/05/06 12:41:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	what_to_print(int status, t_philo **philo)
{
	size_t	time;

	time = get_current_time() - (*philo)->start_time;
	if (status == 3)
	{
		if ((*philo)->status != -1)
			return (printing(time, (*philo)->id, "is eating"), 0);
		else
			return (1);
	}
	else if (status == 4)
	{
		pthread_mutex_lock(&(*philo)->lock);
		if ((*philo)->status != -1)
		{
			pthread_mutex_unlock(&(*philo)->lock);
			return (printing(time, (*philo)->id, "has taken a fork"), 0);
		}
		else
		{
			pthread_mutex_unlock(&(*philo)->lock);
			return (1);
		}
	}
	return (0);
}

size_t	get_current_time(void)
{
	struct timeval	timeval;

	if (gettimeofday(&timeval, NULL) == -1)
		custom_error("gettimeofday() error\n", NULL);
	return (timeval.tv_sec * 1000 + timeval.tv_usec / 1000);
}

void	memset_philo(t_philo *philo, t_program **program, int index)
{
	philo->last_meal_at = 0;
	philo->start_time = 0;
	if (index == (*program)->philo_num - 1)
	{
		if ((*program)->philo_num - 1 != 0)
			(*program)->philos[index].r_fork = &(*program)->forks[0];
		else
			(*program)->philos[index].r_fork = NULL;
	}
	else
		(*program)->philos[index].r_fork = &(*program)->forks[index + 1];
}

void	memset_program(t_program **program)
{
	(*program)->start = 0;
	(*program)->dead = 0;
	(*program)->finished = 0;
	(*program)->start_time = 0;
}

int	ft_atoi(char *nptr, int *nr)
{
	int		sign;
	long	number;

	number = 0;
	sign = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		number = number * 10 + (*nptr - '0');
		if (number * sign > INT_MAX || number * sign < INT_MIN)
			return (1);
		nptr++;
	}
	if (number * sign < 1)
		return (1);
	*nr = (int)(number * sign);
	return (0);
}
