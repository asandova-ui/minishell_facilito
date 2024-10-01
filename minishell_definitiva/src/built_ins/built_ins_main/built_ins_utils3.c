/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 02:29:18 by jamorale          #+#    #+#             */
/*   Updated: 2024/10/01 09:34:36 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

char *ft_strcat(char *dest, const char *src)
{
    char *ptr;

    ptr = dest;
    while (*ptr != '\0')
        ptr++;
    while (*src != '\0')
    {
        *ptr = *src;
        ptr++;
        src++;
    }
    *ptr = '\0';
    return (dest);
}
