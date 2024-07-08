
#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

int main(int argc, char **argv, char **envp)
{
    t_minish mini;
    char *line = NULL;

    (void)argc;
    (void)argv;
    init_struct(&mini, envp);
    if (envp == NULL)
        mini.env_exist = 1;
    else
        init_envp(&mini, envp); // initialize mini.envp with envp
    while (mini.exit == 0)
    {
        line = get_next_line(STDIN_FILENO);
        if (line == NULL)
            break; // Handle EOF or error in get_next_line
        minishell(line, &mini);
        free(line);
    }
    exit(EXIT_SUCCESS);
}

void init_struct(t_minish *mini, char **envp)
{
    mini->in = dup(STDIN_FILENO);
    mini->out = dup(STDOUT_FILENO);
    mini->exit = 0;
    mini->ret_value = 0;
    mini->envp = NULL;
    mini->env_exist = 0;
    mini->envp = envp;
}

void minishell(char *line, t_minish *mini)
{
    built_ins(line, mini);//solo builtins
    //ahora los comandos requeridos
}