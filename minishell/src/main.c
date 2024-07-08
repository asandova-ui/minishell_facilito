
#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

int main(int argc, char **argv, char **envp)
{
    t_minish	*mini;
	char		*line;

    (void)argc;
    (void)argv;
    /*int i = 0;
    while (envp[i] != NULL)
    {
        printf("%s", envp[i]);
    }*/
    mini.in = dup(STDIN);
	mini.out = dup(STDOUT);
    mini->av = argv;
    init_envp(mini, envp);

    if (envp == NULL)
        mini.env_exist = 1;
    else if (envp != NULL)
        init_envp(mini, envp);//en mini.envp ya estan los paths
    while (mini.exit != 0)
    {
        minishell(line = ft_get_next_line(STDIN), mini);
    }
    exit(EXIT_SUCCESS);
    return(0);
}

void minishell(char *line, t_minish	*mini)
{
	built_ins(line, mini);
}
