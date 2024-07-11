
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void	built_ins(char *line, t_minish *mini)
{
	char	*command;
	char	*args;
	int		error;
	char	*home;

	command = strtok(line, " \n");
	if (command == NULL)
		return ;
	if (ft_strcmp(command, "echo") == 0)
	{
		args = strtok(NULL, "\n");
		if (args != NULL && mini->comillas != 1)
			ft_echo(args);
	}
	if (ft_strcmp(command, "cd") == 0)
	{
		args = strtok(NULL, "\n");
		if (args == NULL)
		{
			home = getenv("HOME");
			ft_cd(home, mini);
		}
		else
			ft_cd(args, mini);
	}
	if (ft_strcmp(command, "pwd") == 0)
		ft_pwd();
	if (ft_strcmp(command, "export") == 0)
	{
		args = strtok(NULL, "\n");
		if (args == NULL) // No arguments provided
		{
			ft_export(NULL, mini);
		}
		else
		{
			error = is_valid_env(args);
			if (args[0] == '=')
				error = -3;
			if (error != 1)
			{
				print_error(error, args);
				return ;
			}
			ft_export(args, mini);
		}
	}
	if (ft_strcmp(command, "unset") == 0)
	{
		args = strtok(NULL, "\n");
		if (args != NULL)
			ft_unset(args, mini); // Pasar mini para actualizar envp
	}
	if (ft_strcmp(command, "env") == 0)
		ft_env(mini);
	if (ft_strcmp(command, "exit") == 0)
	{
		args = strtok(NULL, "\n");
		if (args != NULL)
			ft_exit(args);
	}
}

int	ft_strcmp(const char *str1, const char *str2)
{
	while (*str1 && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	return (*(unsigned char *)str1 - *(unsigned char *)str2);
}
