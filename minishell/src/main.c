/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:40:53 by asandova          #+#    #+#             */
/*   Updated: 2024/07/08 09:57:52 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_minish mini;

    (void)argc;
    (void)argv;
    mini.in = dup(STDIN);
	mini.out = dup(STDOUT);
    init_envp(mini, envp);

    return(0);
}