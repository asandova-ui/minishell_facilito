
#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv, char **envp)
{
    t_minish mini;
    t_history *history;
    char *line = NULL;

    (void)argc;
    (void)argv;
    init_struct(&mini, envp);
    history = malloc(sizeof(t_history));
    init_struct_history(history);
    while (mini.exit == 0)
    {
        init_path(&mini);
        sig_init();
        signal(SIGINT, &sig_int);
        signal(SIGQUIT, &sig_quit);
        print_line(&mini);
        line = get_next_line(STDIN_FILENO);
        if (line == NULL)
            break;
        add_to_history(history, line);
        minishell(line, &mini, history);
        free(line);
    }
    exit(EXIT_SUCCESS);
}

void minishell(char *line, t_minish *mini, t_history *history)
{
    char *temp;

    parse_line(line, mini);//vemos que esten cerradas las llaves y comprobamos caracteres especiales: \ $ < >>
    temp = ft_strdup2(line);
    //depurar:

    //ya tienes la linea con control de caracteres especiales y de comillas
    //si el caracter es no reconocible ASCII(-36) significa que es dolar seguido de cosas, es decir toca poner el valor de la variable
    built_ins(temp, mini, history);//solo builtins
    if (mini->exec == 0)
        mini->ret_value = run_command(line,mini);
    //ahora los comandos requeridos
    mini->exec = 0;
}

void add_to_history(t_history *history, char *command)
{
    int i;

    if (history->count < 1000)
        history->history[history->count++] = ft_strdup(command);
    else
    {
        free(history->history[0]);
        i = 1;
        while (i < 1000)
        {
            history->history[i - 1] = history->history[i];
            i++;
        }
        history->history[999] = ft_strdup(command);
    }
    history->i = history->count;
}

void init_struct(t_minish *mini, char **envp)
{
    mini->in = dup(STDIN_FILENO);
    mini->out = dup(STDOUT_FILENO);
    mini->exit = 0;
    mini->ret_value = 0;
    mini->envp = NULL;
    mini->env_exist = 0;
    mini->exec=0;
    if (envp) {
        mini->envp = dup_envp(envp);  // Usar dup_envp para duplicar el entorno
    } else {
        mini->envp = NULL;
    }
}

char **dup_envp(char **envp) {
    int i = 0;
    while (envp[i])
        i++;

    char **new_envp = malloc((i + 1) * sizeof(char *));
    if (!new_envp)
        return NULL;

    for (i = 0; envp[i]; i++) {
        new_envp[i] = ft_strdup(envp[i]);
        if (!new_envp[i]) {
            while (i > 0)
                free(new_envp[--i]);
            free(new_envp);
            return NULL;
        }
    }
    new_envp[i] = NULL;
    return new_envp;
}

void free_envp(char **envp) {
    int i = 0;
    if (envp) {
        while (envp[i]) {
            free(envp[i]);
            i++;
        }
        free(envp);
    }
}