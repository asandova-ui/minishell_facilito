/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 10:06:35 by marvin            #+#    #+#             */
/*   Updated: 2024/05/06 10:06:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <limits.h>

typedef struct s_params
{
	int	amount_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	times_eaten;
}	t_params;

typedef struct s_philo
{
	pthread_t		*ptid;
	int				id;
	int				eat_count;
	int				status;
	size_t			time_to_eat;
	size_t			time_to_die;
	size_t			last_meal_at;
	size_t			time_to_sleep;
	size_t			start_time;
	pthread_mutex_t	lock;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write;
}	t_philo;

typedef struct s_program
{
	pthread_t		*tid;
	int				start;
	int				philo_num;
	int				meals_nr;
	int				dead;
	int				finished;
	t_philo			*philos;
	size_t			death_time;
	size_t			eat_time;
	size_t			sleep_time;
	size_t			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
}	t_program;

int		custom_error(char *string, t_program *data);
int		initialize_program(char **argv, int flag, t_program *program);
size_t	get_current_time(void);
int		set_threads(t_program *program);
void	*philo_routine(void *args);
void	d_mut(t_program *program, int number, int extra);
void	memset_philo(t_philo *philo, t_program **program, int index);
void	destroy_philo_mutex(t_philo *philo, int number);
void	memset_program(t_program **program);
void	f_p(t_program *program);
int		ft_atoi(char *nptr, int *nr);
int		eating(t_philo **philo);
//static int	if_waited_to_long(t_philo **philo);
//static void	unlock_mutexes(int lf, int rf, int w, t_philo **philo);
int		is_eating(t_philo **philo);
int		is_sleeping(t_philo **philo);
int		ft_usleep_sleep(size_t miliseconds, t_philo *philo);
void	finished_eating(t_program **program, int *index);
void	free_philos(t_program *program, int amound);
int		what_to_print(int status, t_philo **philo);
void	printing(size_t time, int id, char *string);
#endif