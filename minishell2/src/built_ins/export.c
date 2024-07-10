
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void ft_export(char *args, t_minish *mini)
{
    char *name;
    char *value;
    char *equal_sign = ft_strchr(args, '=');

    write(1, "hola\n", 5);
    if (equal_sign != NULL)
    {
        // Dividir la cadena en nombre y valor
        *equal_sign = '\0';
        name = args;
        value = equal_sign + 1;
        // Agregar o actualizar la variable de entorno
        add_or_update_env_var(mini, name, value);
    }
    else
    {
        // Solo imprimir el valor de la variable de entorno
        char *env_val = NULL;
        int i = 0;
        int name_len = ft_strlen(args);
        while (mini->envp && mini->envp[i])
        {
            if (ft_strncmp(mini->envp[i], args, name_len) == 0 && mini->envp[i][name_len] == '=')
            {
                env_val = mini->envp[i];
                break;
            }
            i++;
        }

        if (env_val != NULL)
            printf("%s\n", env_val);
        else
            printf("Variable de entorno %s no establecida\n", args);
    }
}

void add_or_update_env_var(t_minish *mini, const char *name, const char *value)
{
    int i = 0;
    int name_len = strlen(name);

    while (mini->envp && mini->envp[i])
    {
        if (strncmp(mini->envp[i], name, name_len) == 0 && mini->envp[i][name_len] == '=')
        {
            // Reemplazar la variable existente
            free(mini->envp[i]);
            mini->envp[i] = malloc(name_len + ft_strlen(value) + 2);
            if (!mini->envp[i]) return;
            sprintf(mini->envp[i], "%s=%s", name, value);
            return;
        }
        i++;
    }
    // Añadir nueva variable
    char **new_envp = realloc(mini->envp, (i + 2) * sizeof(char *));
    if (!new_envp) return;
    mini->envp = new_envp;
    mini->envp[i] = malloc(name_len + ft_strlen(value) + 2);
    if (!mini->envp[i]) return;
    sprintf(mini->envp[i], "%s=%s", name, value);
    mini->envp[i + 1] = NULL;
}

int is_valid_env(const char *env)
{
    int i = 0;
    if (ft_isdigit(env[i]))
        return 0;
    while (env[i] && env[i] != '=')
    {
        if (!ft_isalnum(env[i]) && env[i] != '_')
            return -1;
        i++;
    }
    if (env[i] != '=')
        return 2;
    return 1;
}

int print_error(int error, const char *arg)
{
    if (error == -1)
        ft_putstr_fd("export: not valid in this context: ", STDERR);
    else if (error == 0 || error == -3)
        ft_putstr_fd("export: not a valid identifier: ", STDERR);
    write(STDERR, arg, strlen(arg));
    write(STDERR, "\n", 1);
    return 1;
}
