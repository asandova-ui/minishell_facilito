/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_program.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:12:00 by marvin            #+#    #+#             */
/*   Updated: 2024/05/06 12:12:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	set_forks(t_program *program, int amount)
{
	int	index;

	index = -1;
	while (++index < amount)
	{
		if (pthread_mutex_init(&program->forks[index], NULL) != 0)
			return (index);
	}
	return (-1);
}

static int	set_philos(t_program **program)
{
	int		index;

	index = -1;
	while (++index < (*program)->philo_num)
	{
		memset_philo(&(*program)->philos[index], &(*program), index);
		(*program)->philos[index].ptid = malloc(sizeof(pthread_t));
		if ((*program)->philos[index].ptid == NULL)
			return (0);
		(*program)->philos[index].id = index + 1;
		(*program)->philos[index].time_to_eat = (*program)->eat_time;
		(*program)->philos[index].time_to_die = (*program)->death_time;
		(*program)->philos[index].time_to_sleep = (*program)->sleep_time;
		if (pthread_mutex_init(&((*program)->philos[index]).lock, NULL) != 0)
			return (destroy_philo_mutex((*program)->philos, index),
				free((*program)->philos[index].ptid), 0);
		(*program)->philos[index].l_fork = &(*program)->forks[index];
		(*program)->philos[index].write = &(*program)->write;
	}
	return (1);
}

static int	programa(t_params *params, t_program *program)
{
	int		forks;

	memset_program(&program);
	program->tid = malloc(sizeof(pthread_t));
	if (program->tid == NULL)
		return (f_p(program), 0);
	program->philo_num = params->amount_of_philo;
	program->meals_nr = params->times_eaten;
	program->philos = malloc(sizeof(t_philo) * params->amount_of_philo);
	if (program->philos == NULL)
		return (f_p(program), 0);
	program->death_time = params->time_to_die;
	program->eat_time = params->time_to_eat;
	program->sleep_time = params->time_to_sleep;
	program->forks = malloc(sizeof(pthread_mutex_t) * params->amount_of_philo);
	if (program->forks == NULL)
		return (f_p(program), 0);
	forks = set_forks(program, program->philo_num);
	if (forks != -1)
		return (d_mut(program, forks, 0), f_p(program), 0);
	if (pthread_mutex_init(&program->lock, NULL) != 0)
		return (d_mut(program, program->philo_num, 0), f_p(program), 0);
	if (pthread_mutex_init(&program->write, NULL) != 0)
		return (d_mut(program, program->philo_num, 1), f_p(program), 0);
	return (1);
}

static t_params	*parse_input(char **argv, int flag)
{
	t_params	*params;

	params = malloc(sizeof(t_params));
	if (params == NULL)
		return (NULL);
	if (ft_atoi(argv[1], &params->amount_of_philo))
		return (free(params), NULL);
	if (ft_atoi(argv[2], &params->time_to_die))
		return (free(params), NULL);
	if (ft_atoi(argv[3], &params->time_to_eat))
		return (free(params), NULL);
	if (ft_atoi(argv[4], &params->time_to_sleep))
		return (free(params), NULL);
	if (flag)
	{
		if (ft_atoi(argv[5], &params->times_eaten))
			return (free(params), NULL);
	}
	else
		params->times_eaten = INT_MAX;
	return (params);
}

int	initialize_program(char **argv, int flag, t_program *program)
{
	t_params	*params;

	params = parse_input(argv, flag);
	if (params == NULL)
		return (1);
	if (programa(params, program) == 0)
		return (1);
	free (params);
	if (set_philos(&program) == 0)
		return (d_mut(program, program->philo_num, 2), f_p(program), 1);
	return (0);
}
