
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"
void built_ins(char *line, t_minish *mini)
{
    char *command = strtok(line, " \n");

    if (command == NULL)
        return;

    if (ft_strcmp(command, "echo") == 0)
    {
        char *args = strtok(NULL, "\n");
        if (args != NULL && mini->comillas != 1)
            ft_echo(args);
    }
    else if (ft_strcmp(command, "cd") == 0)
    {
        // Implement cd functionality
    }
    else if (ft_strcmp(command, "pwd") == 0)
    {
        ft_pwd();
    }
    else if (ft_strcmp(command, "export") == 0)
    {
        // Implement export functionality
    }
    else if (ft_strcmp(command, "unset") == 0)
    {
        // Implement unset functionality
    }
    else if (ft_strcmp(command, "env") == 0)
    {
        ft_env(mini->envp);
    }
    else if (ft_strcmp(command, "exit") == 0)
    {
		mini->exit = 1;
        //exit_command();
    }
}

int ft_strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return (*(unsigned char *)str1 - *(unsigned char *)str2);
}