
#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

#include <unistd.h>
#include <stdio.h>

int init_env(t_minish *mini, char **env_array)
{
	t_env	*env;
	t_env	*new;
	int		i;

	if (!(env = malloc(sizeof(t_env))))
		return (1);
	env->value = ft_strdup(env_array[0]);
	env->next = NULL;
	mini->env = env;
	i = 1;
	while (env_array && env_array[0] && env_array[i])
	{
		if (!(new = malloc(sizeof(t_env))))
			return (1);
		new->value = ft_strdup(env_array[i]);
		new->next = NULL;
		env->next = new;
		env = new;
		i++;
	}
	return (0);
}

int main(int argc, char **argv, char **envp)
{
    t_minish mini;
    char *line = NULL;

    (void)argc;
    (void)argv;
    init_struct(&mini);
    init_env(&mini, envp);//cargamos el env en mini a traves del struct s_env
    if (envp == NULL)
        mini.env_exist = 1;
    else
        init_path(&mini, envp); // initialize mini.envp with envp
    while (mini.exit == 0)
    {
        print_line(&mini);
        line = get_next_line(STDIN_FILENO);
        if (line == NULL)
            break; // Handle EOF or error in get_next_line
        minishell(line, &mini);
        free(line);
    }
    exit(EXIT_SUCCESS);
}

void minishell(char *line, t_minish *mini)
{
    line = parse_line(line, mini);//vemos que esten cerradas las llaves y comprobamos caracteres especiales: \ $ < >>
    //depurar:
    //printf("%s", line);

    //ya tienes la linea con control de caracteres especiales y de comillas
    //si el caracter es no reconocible ASCII(-36) significa que es dolar seguido de cosas, es decir toca poner el valor de la variable
    built_ins(line, mini);//solo builtins
    //ahora los comandos requeridos
}

void init_struct(t_minish *mini)
{
    mini->in = dup(STDIN_FILENO);
    mini->out = dup(STDOUT_FILENO);
    mini->exit = 0;
    mini->ret_value = 0;
    mini->env_exist = 0;
}
