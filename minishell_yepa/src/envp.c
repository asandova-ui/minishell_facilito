
#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

void    init_path(t_minish *mini)
{
    char **paths;
    int i;

    i = 0;
	while (mini->envp[i])
	{
		if (!ft_strncmp(mini->envp[i], "PATH=", 5))
			paths = ft_split(mini->envp[i] + 5, ':');
		i++;
	}
    mini->path = paths;
    return;
}