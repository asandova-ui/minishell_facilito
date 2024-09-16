
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void	ft_env(t_minish *mini)
{
    int i = 0;
    while (mini->envp && mini->envp[i])
	{
        printf("%s\n", mini->envp[i]);
        i++;
    }
}
