/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   morethings2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 20:49:34 by alonso            #+#    #+#             */
/*   Updated: 2024/10/01 09:32:19 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_fprintf(const char *str)
{
	write(2, str, strlen(str));
}

bool	es_caracter_especial(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

void	procesar_comilla(const char *input, char *res, t_iandj *iandj,
		bool *in_single_quote)
{
	if (*in_single_quote && es_caracter_especial(input[iandj->i - 1]))
		res[(iandj->j)++] = '"';
	else if (!*in_single_quote && es_caracter_especial(input[iandj->i + 1]))
		res[(iandj->j)++] = '"';
	else
		res[(iandj->j)++] = input[iandj->i];
	*in_single_quote = !*in_single_quote;
}
