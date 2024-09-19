
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

char *handle_quotes(const char *str) {
    char *result = malloc(strlen(str) + 1);  // Asignar tamaño máximo
    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    char *ptr = result;
    const char *start = str;
    int in_single_quote = 0;
    int in_double_quote = 0;

    // Procesar la cadena para manejar comillas
    while (*start) {
        if (*start == '\'' && !in_double_quote) {
            in_single_quote = !in_single_quote;
        } else if (*start == '\"' && !in_single_quote) {
            in_double_quote = !in_double_quote;
        }
        *ptr++ = *start++;
    }
    *ptr = '\0';  // Terminar la cadena procesada

    // Si hay comillas no cerradas, solicitar más entrada
    while (in_single_quote || in_double_quote) {
        printf("quote> ");  // Imprimir el prompt tipo bash
        char *extra_input = readline(NULL);  // Leer entrada adicional
        if (!extra_input) {
            break;  // Si no se recibe más entrada, salir del bucle
        }

        // Reasignar memoria para la nueva entrada
        size_t new_len = strlen(result) + strlen(extra_input) + 1;
        char *new_result = realloc(result, new_len);
        if (new_result == NULL) {
            perror("realloc");
            free(result);
            free(extra_input);
            exit(EXIT_FAILURE);
        }

        result = new_result;
        strcat(result, extra_input);  // Añadir la nueva entrada a la cadena
        free(extra_input);

        // Re-evaluar las comillas en la cadena combinada
        ptr = result;
        in_single_quote = 0;
        in_double_quote = 0;
        while (*ptr) {
            if (*ptr == '\'' && !in_double_quote) {
                in_single_quote = !in_single_quote;
            } else if (*ptr == '\"' && !in_single_quote) {
                in_double_quote = !in_double_quote;
            }
            ptr++;
        }

        // Salir si las comillas se cierran correctamente
        if (!in_single_quote && !in_double_quote) {
            break;
        }
    }

    return result;  // Devolver la cadena procesada
}

void ft_echo(char *line, t_minish *mini) {
    char *arg;
    int new_line = 1;
    char *env_value;
    char *processed_line;

    // Procesar la línea de entrada para manejar las comillas
    processed_line = handle_quotes(line);

    arg = strtok(processed_line, " \n");

    if (arg != NULL && strcmp(arg, "-n") == 0) {
        new_line = 0;
        arg = strtok(NULL, " \n");
    }

    while (arg != NULL) {
        char *current = arg;
        while (*current) {
            if (*current == '$') {
                // Manejar variables de entorno y $? (ret_value)
                if (*(current + 1) == '?') {
                    ft_putnbr_fd(mini->ret_value, 1);
                    current += 2;
                } else {
                    char *var_start = current + 1;
                    char *var_end = var_start;
                    while (*var_end && (*var_end == '_' || isalnum(*var_end)))
                        var_end++;
                    size_t var_len = var_end - var_start;
                    char *var_name = strndup(var_start, var_len);
                    env_value = get_env_value(var_name, mini);
                    free(var_name);
                    if (env_value) {
                        write(1, env_value, strlen(env_value));
                        free(env_value);
                    }
                    current = var_end;
                }
            } else if (*current == '\'' || *current == '\"') {
                // No imprimir las comillas externas balanceadas
                char quote_char = *current;
                current++;
                while (*current && *current != quote_char) {
                    write(1, current, 1);
                    current++;
                }
                if (*current == quote_char) {
                    current++;  // Saltar la comilla de cierre
                }
            } else {
                // Imprimir el contenido fuera de las comillas
                write(1, current, 1);
                current++;
            }
        }
        arg = strtok(NULL, " \n");
        if (arg != NULL) {
            write(1, " ", 1);
        }
    }

    if (new_line)
        write(1, "\n", 1);

    free(processed_line);
}

char *mini_getenv(t_minish *mini, const char *name) {
    int i = 0;
    size_t len = strlen(name);

    while (mini->envp[i] != NULL) {
        if (strncmp(mini->envp[i], name, len) == 0 && mini->envp[i][len] == '=') {
            return mini->envp[i] + len + 1;
        }
        i++;
    }
    return NULL;
}

char *get_env_value(const char *name, t_minish *mini) {
    char *value = mini_getenv(mini, name);

    if (value != NULL)
        return strdup(value);
    else
        return NULL;
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
