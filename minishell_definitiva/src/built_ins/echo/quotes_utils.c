/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:46:01 by jamorale          #+#    #+#             */
/*   Updated: 2024/09/25 05:36:10 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

void	update_quote_state(const char *str, t_quote_state *state)
{
	while (*str)
	{
		if (*str == '\'' && !state->in_double_quote)
			state->in_single_quote = !state->in_single_quote;
		else if (*str == '\"' && !state->in_single_quote)
			state->in_double_quote = !state->in_double_quote;
		str++;
	}
}

size_t	calculate_new_length(const char *result, const char *extra_input,
		int is_first_input)
{
	size_t	new_len;

	new_len = strlen(result) + strlen(extra_input);
	if (!is_first_input)
		new_len += 1;
	return (new_len + 1);
}

char	*allocate_new_result(char *result, const char *extra_input,
		int is_first_input)
{
	size_t	new_len;
	char	*new_result;

	new_len = calculate_new_length(result, extra_input, is_first_input);
	new_result = realloc(result, new_len);
	if (!new_result)
	{
		perror("realloc");
		free(result);
		free((char *)extra_input);
		exit(EXIT_FAILURE);
	}
	return (new_result);
}

void	append_input(char **result, const char *extra_input, int is_first_input)
{
	if (!is_first_input)
		strcat(*result, "\n");
	strcat(*result, extra_input);
}

char	*initialize_result(const char *str)
{
	char	*result;

	result = malloc(strlen(str) + 1);
	if (!result)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	strcpy(result, str);
	return (result);
}
