
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

char *handle_quotes(const char *str) {
    char *result = malloc(strlen(str) + 1);  // Allocate max size
    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    char *ptr = result;
    const char *start = str;
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (*start) {
        if (*start == '\'' && !in_double_quote) {
            in_single_quote = !in_single_quote;
            start++;
        } else if (*start == '\"' && !in_single_quote) {
            in_double_quote = !in_double_quote;
            start++;
        } else {
            *ptr++ = *start++;
        }
    }
    *ptr = '\0';
    return result;
}

void ft_echo(char *line, t_minish *mini) {
    char *arg;
    int new_line = 1;
    char *env_value;
    char *processed_line;

    // Process input line to handle quotes
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
            } else {
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
