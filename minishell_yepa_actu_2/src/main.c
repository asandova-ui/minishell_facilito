
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
    g_mini = &mini;
    history = malloc(sizeof(t_history));
     if (!history) {
        // Handle malloc failure
        free_envp(mini.envp);
        exit(EXIT_FAILURE);
    }
    setup_signals();
    init_struct_history(history);
    while (mini.exit == 0)
    {
        init_path(&mini);
        sig_init();
        //setup_signals();
        //print_line(&mini);
        line = readline("\033[1;32m→ minishell ▸ \033[0m");
        if (line == NULL)
            break;
        add_to_history(history, line);
        minishell(line, &mini, history);
        free(line);
        line = NULL;
        if (mini.path) {
            free_paths(mini.path);
            mini.path = NULL;
        }
    }
    free_history(history);
    free_envp(mini.envp);
    exit(EXIT_SUCCESS);
}

void minishell(char *line, t_minish *mini, t_history *history)
{
    char *temp;
    temp = ft_strdup2(line);
    //depurar:

    built_ins(temp, mini, history);//solo builtins
    if (mini->exec == 0)
        mini->ret_value = run_command(line,mini);

    
    //ahora los comandos requeridos
    free(temp);
    mini->exec = 0;
}

void add_to_history(t_history *history, char *command)
{
    // Añadir el comando al historial de readline
    add_history(command);

    // Manejar el límite del tamaño del historial si es necesario
    // Puedes ajustar esto según sea necesario
    if (history->count < 1000) {
        history->history[history->count++] = strdup(command);
    } else {
        free(history->history[0]);
        for (int i = 1; i < 1000; i++) {
            history->history[i - 1] = history->history[i];
        }
        history->history[999] = strdup(command);
    }
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
