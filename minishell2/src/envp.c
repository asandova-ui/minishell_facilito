
#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

void    init_envp(t_minish *mini, char *envp[])
{
    char **paths;
    int i;

    i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			paths = ft_split(envp[i] + 5, ':');
		i++;
	}
    mini->path = paths;
    return;
}