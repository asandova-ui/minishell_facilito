/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:12:06 by alonso            #+#    #+#             */
/*   Updated: 2024/10/01 09:20:34 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

size_t	ft_strcspn(const char *str, const char *reject)
{
	const char	*s = str;
	const char	*r;

	while (*s != '\0')
	{
		r = reject;
		while (*r != '\0')
		{
			if (*s == *r)
			{
				return (s - str);
			}
			r++;
		}
		s++;
	}
	return (s - str);
}

char	*ft_strjoin_3args(char const *s1, char connector, char const *s2)
{
	char	*str;
	size_t	x;
	size_t	y;

	if (!s1 || !s2)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 2));
	if (!str)
		return (NULL);
	x = 0;
	y = 0;
	while (s1[y])
		str[x++] = s1[y++];
	str[x++] = connector;
	y = 0;
	while (s2[y])
		str[x++] = s2[y++];
	str[x] = '\0';
	return (str);
}

void	init_struct_history(t_history *history)
{
	int	j;

	j = 0;
	history->count = 0;
	history->i = -1;
	while (j < 1000)
	{
		history->history[j] = NULL;
		j++;
	}
}

void	init_env_var(t_env_var *env_var)
{
	env_var->name = NULL;
	env_var->value = NULL;
	env_var->var_len = 0;
}

void	init_redirection(t_redirection *red)
{
	red->infile = NULL;
	red->outfile = NULL;
	red->heredoc_delim = NULL;
	red->append_mode = 0;
}
