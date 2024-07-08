
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"
void	built_ins(char *line, t_minish	*mini)
{
    int i;
    char *str;

	str = NULL;
    i = 0;
    while (line[i] != 32)
    {
        str[i] = line[i];
        i++;
    }
	if (ft_strcmp(str, "echo") == 0)
	{
	}
	if (ft_strcmp(str, "cd") == 0)
	{
	}
	if (ft_strcmp(str, "pwd") == 0)
        ft_pwd();
	if (ft_strcmp(str, "export") == 0)
	{
	}
	if (ft_strcmp(str, "unset") == 0)
	{
	}
	if (ft_strcmp(str, "env") == 0) //DEBERIA FUNCIONAR
		ft_env(mini->envp);
	if (ft_strcmp(str, "exit") == 0)
		exit_command();
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

void	exit_command(void)
{
	exit(EXIT_SUCCESS);
}
