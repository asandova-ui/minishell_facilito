
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void	ft_echo(char *line, t_minish *mini)
{
	char	*arg;
	int		new_line;
	char	*env_value;
	char	*current;
	char	*var_start;
	char	*var_end;
	size_t	var_len;
	char	*var_name;

	new_line = 1;
	arg = strtok(line, " \n");
	if (arg != NULL && ft_strcmp(arg, "-n") == 0)
	{
		new_line = 0;
		arg = strtok(NULL, " \n");
	}
	while (arg != NULL)
	{
		current = arg;
		while (*current)
		{
			if (*current == '$')
			{
				if (*(current + 1) == '?')
				{
					ft_putnbr_fd(mini->ret_value, 1);
					current += 2;
				}
				else
				{
					var_start = current + 1;
					var_end = var_start;
					while (*var_end && (*var_end == '_' || ft_isalnum(*var_end)))
						var_end++;
					var_len = var_end - var_start;
					var_name = ft_strndup(var_start, var_len);
					env_value = get_env_value(var_name);
					free(var_name);
					if (env_value)
					{
						write(1, env_value, ft_strlen(env_value));
						free(env_value);
					}
					current = var_end;
				}
			}
			else
			{
				write(1, current, 1);
				current++;
			}
		}
		arg = strtok(NULL, " \n");
		if (arg != NULL)
		{
			write(1, " ", 1);
		}
	}
	if (new_line)
		write(1, "\n", 1);
}

char	*get_env_value(const char *name)
{
	char	*value;

	value = getenv(name);
	if (value != NULL)
		return (ft_strdup(value));
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
