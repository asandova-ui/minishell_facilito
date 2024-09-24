/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:45:15 by jamorale          #+#    #+#             */
/*   Updated: 2024/09/23 18:45:33 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

char	*mini_getenv(t_minish *mini, const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = strlen(name);
	while (mini->envp[i] != NULL)
	{
		if (strncmp(mini->envp[i], name, len) == 0 && mini->envp[i][len] == '=')
		{
			return (mini->envp[i] + len + 1);
		}
		i++;
	}
	return (NULL);
}

char	*get_env_value(const char *name, t_minish *mini)
{
	char	*value;

	value = mini_getenv(mini, name);
	if (value != NULL)
		return (strdup(value));
	else
		return (NULL);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(s);
	if (n > len)
		n = len;
	dup = (char *)malloc(n + 1);
	if (dup == NULL)
		return (NULL);
	ft_strncpy(dup, s, n);
	dup[n] = '\0';
	return (dup);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

void	initialize_echo_state(EchoState *state, char *line, t_minish *mini)
{
	state->processed_line = handle_quotes(line);
	state->new_line = 1;
	state->mini = mini;
}
