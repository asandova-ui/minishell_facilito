
#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

int main(int argc, char **argv, char **envp)
{
    t_minish	mini;
	char		*line;

    (void)argc;
    (void)argv;
    init_struct (&mini);
    if (envp == NULL)
        mini.env_exist = 1;
    else if (envp != NULL)
        init_envp(&mini, envp);//en mini.envp ya estan los paths
    while (mini.exit != 0)
    {
        minishell(line = get_next_line(STDIN), &mini);
    }
    exit(EXIT_SUCCESS);
    return(0);
}

void init_struct(t_minish *mini)
{
    mini->in = dup(STDIN);
	mini->out = dup(STDOUT);
    mini->exit = 0;
    mini->ret_value = 0;
}
void minishell(char *line, t_minish	*mini)
{
	built_ins(line, mini);
}

/*int i = 0;
    while (envp[i] != NULL)
    {
        printf("%s", envp[i]);
    }*/
