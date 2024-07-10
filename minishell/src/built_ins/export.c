#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

//si la linea es correcta crea variable y actualiza el mini->env
//si no es correcta o no hace nada, o da un error
int		is_valid_env(const char *env)
{
	int		i;

	i = 0;
	if (ft_isdigit(env[i]) == 1)
		return (0);
	while (env[i] && env[i] != '=')
	{
		if (ft_isalnum(env[i]) == 0)
			return (-1);
		i++;
	}
	if (env[i] != '=')
		return (2);
	return (1);
}

static int	print_error(int error, const char *arg)
{
	int		i;

	if (error == -1)
		ft_putstr_fd("export: not valid in this context: ", STDERR);
	else if (error == 0 || error == -3)
		ft_putstr_fd("export: not a valid identifier: ", STDERR);
	i = 0;
	while (arg[i] && (arg[i] != '=' || error == -3))
	{
		write(STDERR, &arg[i], 1);
		i++;
	}
	write(STDERR, "\n", 1);
	return (1);
}

int			env_add(const char *value, t_env *env)
{
	t_env	*new;
	t_env	*tmp;
    write(1, "b", 1);
	if (env && env->value == NULL)
	{
		env->value = ft_strdup(value);
		return (0);
	}
	if (!(new = malloc(sizeof(t_env))))
		return (-1);
	new->value = ft_strdup(value);
	while (env && env->next && env->next->next)
		env = env->next;
	tmp = env->next;
	env->next = new;
	new->next = tmp;
	return (0);
}

char		*get_env_name(char *dest, const char *src)
{
	int		i;

	i = 0;
	while (src[i] && src[i] != '=' && ft_strlen(src) < BUFF_SIZE)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int			is_in_env(t_env *env, char *args)
{
	char	var_name[BUFF_SIZE];
	char	env_name[BUFF_SIZE];

	get_env_name(var_name, args);
	while (env && env->next)
	{
		get_env_name(env_name, env->value);
		if (ft_strcmp(var_name, env_name) == 0)
		{
			free(env->value);
			env->value = ft_strdup(args);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

int			ft_export(char *line, t_minish *mini)
{
	int		new_env;
	int		error_ret;
    char **args;
    //write(1, "a", 1);
	new_env = 0;
    args = ft_split(line, '\n');
    printf("%s", args[0]);
    printf("%s", args[1]);
    //printf("%s", args[2]);
	if (!args[1])
	{
		//print_sorted_env(secret);
		return (0);
	}
	else
	{
		error_ret = is_valid_env(args[1]);
		if (args[1][0] == '=')
			error_ret = -3;
		if (error_ret <= 0)
			return (print_error(error_ret, args[1]));
		new_env = error_ret == 2 ? 1 : is_in_env(mini->env, args[1]);
		if (new_env == 0)
		{
			if (error_ret == 1)
				env_add(args[1], mini->env);
			env_add(args[1], mini->env);
		}
	}
	return (0);
}