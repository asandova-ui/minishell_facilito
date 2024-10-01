/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 02:29:18 by jamorale          #+#    #+#             */
/*   Updated: 2024/09/24 03:25:17 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

char *ft_strtok(char* str, const char* delim) {
    static char* last_token = NULL;
    char* token_start;

    // Si str es NULL, continuamos desde donde lo dejamos la última vez
    if (str == NULL) {
        str = last_token;
    }

    // Si no hay más string para tokenizar, retornamos NULL
    if (str == NULL || *str == '\0') {
        return NULL;
    }

    // Saltamos los delimitadores iniciales
    str += strspn(str, delim);
    if (*str == '\0') {
        return NULL;
    }

    // Encontramos el final del token
    token_start = str;
    str = strpbrk(str, delim);
    if (str == NULL) {
        // Este es el último token
        last_token = NULL;
    } else {
        // Marcamos el final del token y actualizamos last_token
        *str = '\0';
        last_token = str + 1;
    }

    return token_start;
}