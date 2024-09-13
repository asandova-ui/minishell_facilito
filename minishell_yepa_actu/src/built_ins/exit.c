

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void	ft_exit(char *args)
{
	char	*arg;
	int		exit_value;

	if (args == NULL || *args == '\0')
		exit(0);
	int num_args = 0;
	char *temp_args = strdup(args);
	if (!temp_args)
	{
		perror("strdup");
		exit(1);
	}
	arg = strtok(temp_args, " \n");
	while (arg != NULL)
	{
		num_args++;
		arg = strtok(NULL, " \n");
	}
	free(temp_args);
	if (num_args > 1)
	{
		fprintf(stderr, "exit\nbash: exit: too many arguments\n");
		return;
	}
	if (!is_valid_number(args))
		fprintf(stderr, "exit\nbash: exit: %s: numeric argument required\n", args);
	else
	{
		exit_value = ft_atoi(args);
		exit(exit_value);
	}
}

int	is_valid_number(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}
