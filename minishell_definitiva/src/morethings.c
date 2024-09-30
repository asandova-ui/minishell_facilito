/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   morethings.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 19:29:00 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 21:03:15 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	copiar_caracter(const char *input, char *res, t_iandj *iandj)
{
	res[(iandj->j)++] = input[(iandj->i)];
}

char	*reemplazar_comillas(const char *input)
{
	int		len;
	char	*res;
	bool	in_single_quote;
	t_iandj	iandj;

	len = strlen(input);
	res = (char *)malloc((len + 1) * sizeof(char));
	iandj.i = 0;
	iandj.j = 0;
	in_single_quote = false;
	while (input[iandj.i] != '\0')
	{
		if (input[iandj.i] == '\'')
			procesar_comilla(input, res, &iandj, &in_single_quote);
		else
			copiar_caracter(input, res, &iandj);
		iandj.i++;
	}
	res[iandj.j] = '\0';
	return (res);
}

int	es_una_palabra(const char *input, int len)
{
	int	i;

	i = 1;
	while (i < len - 1)
	{
		if (isspace(input[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*more_checks(const char *input)
{
	int		len;
	char	*res;

	len = strlen(input);
	if (len >= 2 && input[0] == '\"' && input[len - 1] == '\"'
		&& es_una_palabra(input, len))
	{
		res = (char *)malloc((len - 1) * sizeof(char));
		strncpy(res, input + 1, len - 2);
		res[len - 2] = '\0';
		return (res);
	}
	res = (char *)malloc((len + 1) * sizeof(char));
	strcpy(res, input);
	return (res);
}

char	*morethings(char *line)
{
	line = more_checks(line);
	return (line);
}
