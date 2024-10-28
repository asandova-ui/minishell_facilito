/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 10:04:42 by marvin            #+#    #+#             */
/*   Updated: 2024/05/06 10:04:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int argc, char **argv)
{
	t_program	*program;

	if (!(argc == 5 || argc == 6))
		return (custom_error("Numero de args incorrecto\n", NULL), 1);
	program = malloc (sizeof(t_program));
	if (program == NULL)
		return (1);
	if (initialize_program(argv, argc == 6, program))
	{
		free(program);
		return (1);
	}
	set_threads(program);
	destroy_philo_mutex(program->philos, program->philo_num);
	d_mut(program, program->philo_num, 2);
	f_p(program);
	return (0);
}
