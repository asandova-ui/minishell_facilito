/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_things.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 10:51:13 by alonso            #+#    #+#             */
/*   Updated: 2024/09/18 14:28:46 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

void free_paths(char **paths)
{
    int i = 0;
    if (paths)
    {
        while (paths[i])
        {
            free(paths[i]);
            i++;
        }
        free(paths);
    }
}

void free_history(t_history *history)
{
    for (int i = 0; i < history->count; i++) {
        free(history->history[i]);
    }
    free(history);
}