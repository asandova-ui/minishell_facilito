/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 02:29:18 by jamorale          #+#    #+#             */
/*   Updated: 2024/10/01 09:35:44 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

int	ft_strcmp(const char *str1, const char *str2)
{
	while (*str1 && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	return (*(unsigned char *)str1 - *(unsigned char *)str2);
}

void	show_history(t_history *history, t_minish *mini)
{
	int	i;

	i = 0;
	while (i < history->count)
	{
		ft_printf("%d %s\n", i + 1, history->history[i]);
		i++;
	}
	mini->exec = 1;
}

bool	has_redirection(const char *str)
{
	bool	in_quotes;
	char	quote_char;

	in_quotes = false;
	quote_char = '\0';
	while (*str)
	{
		if (*str == '"' || *str == '\'')
		{
			if (quote_char == '\0')
			{
				quote_char = *str;
				in_quotes = true;
			}
			else if (quote_char == *str)
			{
				quote_char = '\0';
				in_quotes = false;
			}
		}
		else if ((*str == '>' || *str == '<') && !in_quotes)
			return (true);
		str++;
	}
	return (false);
}

char	*redirect_echo(char *line)
{
	char		*new_line;
	size_t		len;
	char		*echo_position;
	const char	*bin_echo = "/bin/echo";

	new_line = NULL;
	echo_position = ft_strstr(line, "echo");
	if (echo_position == NULL)
		return (NULL);
	if (has_redirection(line))
	{
		len = ft_strlen(line) + ft_strlen(bin_echo) - ft_strlen("echo") + 1;
		new_line = (char *)malloc(len);
		if (new_line == NULL)
		{
			perror("malloc");
			return (NULL);
		}
		ft_strncpy(new_line, line, echo_position - line);
		new_line[echo_position - line] = '\0';
		ft_strcat(new_line, bin_echo);
		ft_strcat(new_line, echo_position + ft_strlen("echo"));
		return (new_line);
	}
	return (NULL);
}

void	handle_unset(t_cmd_ctx *ctx, t_minish *mini)
{
	int	ret_value;

	ctx->args = ft_strtok(NULL, "\n");
	if (ctx->args == NULL)
	{
		mini->ret_value = 0;
		return ;
	}
	ret_value = ft_unset(ctx->args, mini);
	mini->ret_value = ret_value;
	mini->exec = 1;
}
