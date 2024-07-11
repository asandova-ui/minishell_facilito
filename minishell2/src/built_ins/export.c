
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void	ft_export(char *args, t_minish *mini)
{
	char	*name;
	char	*value;
	char	*equal_sign;
	char	*env_val;
	int		i;
	int		name_len;

	if (args == NULL)
	{
		print_sorted_envp(mini);
		return ;
	}
	equal_sign = ft_strchr(args, '=');
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
		env_val = NULL;
		i = 0;
		name_len = ft_strlen(args);
		while (mini->envp && mini->envp[i])
		{
			if (ft_strncmp(mini->envp[i], args, name_len) == 0
				&& mini->envp[i][name_len] == '=')
			{
				env_val = mini->envp[i];
				break ;
			}
			i++;
		}
		if (env_val != NULL)
			printf("%s\n", env_val);
		else
			printf("Variable de entorno %s no establecida\n", args);
	}
}

void	add_or_update_env_var(t_minish *mini, const char *name,
		const char *value)
{
	int		i;
	int		name_len;
	char	**new_envp;

	i = 0;
	name_len = strlen(name);
	while (mini->envp && mini->envp[i])
	{
		if (strncmp(mini->envp[i], name, name_len) == 0
			&& mini->envp[i][name_len] == '=')
		{
			// Reemplazar la variable existente
			free(mini->envp[i]);
			mini->envp[i] = malloc(name_len + ft_strlen(value) + 2);
			if (!mini->envp[i])
				return ;
			sprintf(mini->envp[i], "%s=%s", name, value);
			return ;
		}
		i++;
	}
	// Añadir nueva variable
	new_envp = realloc(mini->envp, (i + 2) * sizeof(char *));
	if (!new_envp)
		return ;
	mini->envp = new_envp;
	mini->envp[i] = malloc(name_len + ft_strlen(value) + 2);
	if (!mini->envp[i])
		return ;
	sprintf(mini->envp[i], "%s=%s", name, value);
	mini->envp[i + 1] = NULL;
}

int	is_valid_env(const char *env)
{
	int	i;

	i = 0;
	if (ft_isdigit(env[i]))
		return (0);
	while (env[i] && env[i] != '=')
	{
		if (!ft_isalnum(env[i]) && env[i] != '_')
			return (-1);
		i++;
	}
	if (env[i] != '=')
		return (2);
	return (1);
}

int	print_error(int error, const char *arg)
{
	if (error == -1)
		ft_putstr_fd("export: not valid in this context: ", STDERR);
	else if (error == 0 || error == -3)
		ft_putstr_fd("export: not a valid identifier: ", STDERR);
	write(STDERR, arg, strlen(arg));
	write(STDERR, "\n", 1);
	return (1);
}

void	print_sorted_envp(t_minish *mini)
{
	char	**sorted_envp;
	int		i;
	int		env_count;

	env_count = 0;
	while (mini->envp[env_count])
		env_count++;
	sorted_envp = malloc((env_count + 1) * sizeof(char *));
	if (!sorted_envp)
		return ;
	i = 0;
	while (i < env_count)
	{
		sorted_envp[i] = mini->envp[i];
		i++;
	}
	sorted_envp[env_count] = NULL;
	ft_qsort(sorted_envp, env_count, sizeof(char *), compare_env);
	i = 0;
	while (i < env_count)
	{
		printf("%s\n", sorted_envp[i]);
		i++;
	}
	free(sorted_envp);
}

int	compare_env(const void *a, const void *b)
{
	return (ft_strcmp(*(const char **)a, *(const char **)b));
}

void	ft_qsort(void *base, size_t nitems, size_t size,
		int (*compar)(const void *, const void *))
{
	char	*pivot;
	void	*tmp;

	size_t i, j;
	if (nitems < 2)
		return ;
	pivot = base + (nitems / 2) * size;
	i = 0;
	j = nitems - 1;
	while (1)
	{
		while (compar(base + i * size, pivot) < 0)
			i++;
		while (compar(base + j * size, pivot) > 0)
			j--;
		if (i >= j)
			break ;
		tmp = malloc(size);
		if (!tmp)
			return ;
		ft_memcpy(tmp, base + i * size, size);
		ft_memcpy(base + i * size, base + j * size, size);
		ft_memcpy(base + j * size, tmp, size);
		free(tmp);
		if (pivot == base + i * size)
			pivot = base + j * size;
		else if (pivot == base + j * size)
			pivot = base + i * size;
		i++;
		j--;
	}
	ft_qsort(base, i, size, compar);
	ft_qsort(base + (i * size), nitems - i, size, compar);
}
