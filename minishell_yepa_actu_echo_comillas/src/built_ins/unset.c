
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void	ft_unset(char *args, t_minish *mini)
{
	char	*arg;

	arg = strtok(args, " \n");
	while (arg != NULL)
	{
		remove_env_var(mini, arg);
		arg = strtok(NULL, " \n");
		//init_path(mini);
	}
}

void	remove_env_var(t_minish *mini, const char *name)
{
	int	i;
	int	j;
	int	name_len;

	i = 0;
	name_len = ft_strlen(name);
	while (mini->envp && mini->envp[i])
	{
		if (ft_strncmp(mini->envp[i], name, name_len) == 0
			&& mini->envp[i][name_len] == '=')
		{
			free(mini->envp[i]);
			j = i;
			while (mini->envp[j])
			{
				mini->envp[j] = mini->envp[j + 1];
				j++;
			}
			continue ;
		}
		i++;
	}
}
