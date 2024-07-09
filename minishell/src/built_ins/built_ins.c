
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"
void built_ins(char *line, t_minish *mini)
{
    char *str = strtok(line, " \n");
    
    if (str == NULL)
        return;

    else if (ft_strcmp(str, "echo") == 0)
    {
        // Implement echo functionality
    }
    else if (ft_strcmp(str, "cd") == 0)
    {
        // Implement cd functionality
    }
    else if (ft_strcmp(str, "pwd") == 0)
    {
        ft_pwd();
    }
    else if (ft_strcmp(str, "export") == 0)
    {
        // Implement export functionality
    }
    else if (ft_strcmp(str, "unset") == 0)
    {
        // Implement unset functionality
    }
    else if (ft_strcmp(str, "env") == 0)
    {
        ft_env(mini->envp);
    }
    else if (ft_strcmp(str, "exit") == 0)
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