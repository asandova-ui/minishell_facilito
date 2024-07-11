
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void ft_unset(char *name, t_minish *mini)
{
    remove_env_var(mini, name);
}
void remove_env_var(t_minish *mini, const char *name)
{
    int i = 0;
    int name_len = ft_strlen(name);

    while (mini->envp && mini->envp[i])
    {
        if (ft_strncmp(mini->envp[i], name, name_len) == 0 && mini->envp[i][name_len] == '=')
        {
            free(mini->envp[i]);
            while (mini->envp[i])
            {
                mini->envp[i] = mini->envp[i + 1];
                i++;
            }
            return;
        }
        i++;
    }
}
