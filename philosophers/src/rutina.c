/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutina.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:40:06 by marvin            #+#    #+#             */
/*   Updated: 2024/05/06 12:40:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static t_program	*set_start(t_program *program)
{
	int	index;

	if (!program->start_time)
	{
		program->start_time = get_current_time();
		index = -1;
		while (++index < program->philo_num)
		{
			pthread_mutex_lock(&program->philos[index].lock);
			program->philos[index].last_meal_at = program->start_time;
			program->philos[index].start_time = program->start_time;
			program->philos[index].status = 1;
			pthread_mutex_unlock(&program->philos[index].lock);
		}
		return (program);
	}
	return (NULL);
}

static int	check_finished(t_program **program, int finished, int number)
{
	int	index;

	if ((*program)->dead || (*program)->finished == (*program)->philo_num)
	{
		index = 0;
		while (index < (*program)->philo_num)
		{
			pthread_mutex_lock(&(*program)->philos[index].lock);
			(*program)->philos[index].status = -1;
			pthread_mutex_unlock(&(*program)->philos[index].lock);
			index++;
		}
		if (finished == 0)
		{
			pthread_mutex_lock(&(*program)->write);
			printf("%zu %d died\n", get_current_time() - (*program)->start_time,
				(*program)->philos[number].id);
			pthread_mutex_unlock(&(*program)->write);
		}
		return (1);
	}
	return (0);
}

static void	check_death(t_program **program, int index)
{
	while (1)
	{
		pthread_mutex_lock(&(*program)->philos[index].lock);
		if ((*program)->philos[index].status == -1)
			(*program)->dead = 1;
		pthread_mutex_unlock(&(*program)->philos[index].lock);
		if ((*program)->dead || (*program)->finished == (*program)->philo_num)
			if (check_finished(program, (*program)->finished, index))
				break ;
		pthread_mutex_lock(&(*program)->philos[index].lock);
		if ((get_current_time() - (*program)->philos[index].last_meal_at)
			>= (*program)->death_time)
		{
			(*program)->dead = 1;
			pthread_mutex_unlock(&(*program)->philos[index].lock);
			check_finished(program, (*program)->finished, index);
			break ;
		}
		if (!(*program)->dead
			&& (*program)->philos[index].eat_count >= (*program)->meals_nr)
			(*program)->finished++;
		pthread_mutex_unlock(&(*program)->philos[index].lock);
		finished_eating(&(*program), &index);
		index++;
	}
}

static void	*checker_routine(void *args)
{
	t_program	*program;

	program = (t_program *)args;
	while (1)
	{
		pthread_mutex_lock(&program->lock);
		if (program->start == 1)
		{
			pthread_mutex_unlock(&program->lock);
			break ;
		}
		pthread_mutex_unlock(&program->lock);
		usleep(500);
	}
	program = set_start(program);
	if (program != NULL)
		check_death(&program, 0);
	return (NULL);
}

int	set_threads(t_program *program)
{
	int	index;

	if (pthread_create(program->tid, NULL, checker_routine, program) != 0)
		return (custom_error("thread create failed", program));
	index = -1;
	while (++index < program->philo_num)
	{
		if (pthread_create(program->philos[index].ptid, NULL, philo_routine,
				&program->philos[index]) != 0)
			return (custom_error("philo thread create failed", program));
	}
	pthread_mutex_lock(&program->lock);
	program->start = 1;
	pthread_mutex_unlock(&program->lock);
	if (pthread_join(*program->tid, NULL) != 0)
		return (custom_error("thread_join", program));
	index = -1;
	while (++index < program->philo_num)
		pthread_join(*program->philos[index].ptid, NULL);
	return (0);
}
