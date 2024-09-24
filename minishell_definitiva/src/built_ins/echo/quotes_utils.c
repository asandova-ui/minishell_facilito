/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:46:01 by jamorale          #+#    #+#             */
/*   Updated: 2024/09/24 03:03:02 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

char	*handle_quotes(const char *str)
{
	char		*ptr;
	const char	*start = str;
	int			in_single_quote;
	int			in_double_quote;
	size_t		new_len;
	char		*new_result;
	char		*result;
	char		*extra_input;

	result = malloc(strlen(str) + 1);
	if (result == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	ptr = result;
	in_single_quote = 0;
	in_double_quote = 0;
	while (*start)
	{
		if (*start == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
		}
		else if (*start == '\"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
		}
		*ptr++ = *start++;
	}
	*ptr = '\0';
	while (in_single_quote || in_double_quote)
	{
		printf("quote> ");
		extra_input = readline(NULL);
		if (!extra_input)
		{
			break ;
		}
		new_len = strlen(result) + strlen(extra_input) + 1;
		new_result = realloc(result, new_len);
		if (new_result == NULL)
		{
			perror("realloc");
			free(result);
			free(extra_input);
			exit(EXIT_FAILURE);
		}
		result = new_result;
		strcat(result, extra_input);
		free(extra_input);
		ptr = result;
		in_single_quote = 0;
		in_double_quote = 0;
		while (*ptr)
		{
			if (*ptr == '\'' && !in_double_quote)
			{
				in_single_quote = !in_single_quote;
			}
			else if (*ptr == '\"' && !in_single_quote)
			{
				in_double_quote = !in_double_quote;
			}
			ptr++;
		}
		if (!in_single_quote && !in_double_quote)
		{
			break ;
		}
	}
	return (result);
}
