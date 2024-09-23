/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:34:43 by alonso            #+#    #+#             */
/*   Updated: 2024/09/23 13:35:42 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

char	*handle_quotes(const char *str)
{
	char		*ptr;
	const char	*start = str;
	int			in_single_quote;
	int			in_double_quote;
	size_t		new_len;
	char		*new_result;

	char *result = malloc(strlen(str) + 1); // Asignar tamaño máximo
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
		char *extra_input = readline(NULL);
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

int	ft_echo(char *line, t_minish *mini)
{
	char	*arg;
	int		new_line = 1;
	char	*processed_line;

	processed_line = handle_quotes(line);
	if (processed_line == NULL)
		return (1);

	// Procesa el primer argumento, si es "-n" no agregamos un salto de línea.
	arg = strtok(processed_line, " \n");
	if (arg == NULL) {
		// Si no hay argumentos, imprimimos solo un salto de línea
		write(1, "\n", 1);
		free(processed_line);
		return (0);
	}

	if (arg != NULL && strcmp(arg, "-n") == 0) {
		new_line = 0;
		arg = strtok(NULL, " \n");
	}

	while (arg != NULL) {
		char *current = arg;
		int in_single_quote = 0;
		int in_double_quote = 0;

		while (*current) {
			if (*current == '\'') {
				// Ingresamos o salimos de comillas simples
				in_single_quote = !in_single_quote;
				write(1, current, 1);  // Imprimimos la comilla simple
				current++;
			}
			else if (*current == '\"') {
				// Ingresamos o salimos de comillas dobles
				in_double_quote = !in_double_quote;
				current++;  // No imprimimos las comillas dobles
			}
			else if (*current == '$' && in_double_quote) {
				// Expansión de variables dentro de comillas dobles (pero no comillas simples)
				char *var_start = current + 1;
				char *var_end = var_start;
				while (*var_end && (*var_end == '_' || isalnum(*var_end)))
					var_end++;
				size_t var_len = var_end - var_start;
				char *var_name = strndup(var_start, var_len);
				char *env_value = mini_getenv(mini, var_name);
				free(var_name);
				if (env_value) {
					write(1, env_value, strlen(env_value));
				}
				current = var_end;
			}
			else {
				// Imprimimos el resto de los caracteres
				write(1, current, 1);
				current++;
			}
		}

		arg = strtok(NULL, " \n");
		if (arg != NULL)
			write(1, " ", 1);
	}

	if (new_line)
		write(1, "\n", 1);

	free(processed_line);
	return (0);
}

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
