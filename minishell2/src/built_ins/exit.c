

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void	ft_exit(char *args)
{
	char	*arg;
	char	*temp_args;
	int		exit_value;
	int		num_args;

	num_args = 0;
	temp_args = strdup(args);
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
		write(1, "exit\n", 5);
		fprintf(stderr, "exit: too many arguments\n");
        return ;
	}
	if (args == NULL || *args == '\0')
		exit(0);
	if (!is_valid_number(args))
	{
		write(1, "exit\n", 5);
		fprintf(stderr, "exit: %s: numeric argument required\n", args);
        return ;
	}
	exit_value = ft_atoi(args);
	if (exit_value < 0 || exit_value > MAX_EXIT_VALUE)
	{
		write(1, "exit\n", 5);
		fprintf(stderr, "exit: %s: numeric argument required\n", args);
        return ;
	}
	exit(exit_value);
}

int	is_valid_number(const char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}
