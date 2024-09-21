
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

int ft_env(t_minish *mini)
{
    int i = 0;
    while (mini->envp && mini->envp[i])
    {
        if (printf("%s\n", mini->envp[i]) < 0)
            return 1;
        i++;
    }
    return 0;
}