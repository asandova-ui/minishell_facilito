#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void print_line(t_minish *mini)
{
    //eliminada la primera linea (borraba cositas del terminal)
    if (mini->ret_value == 0)
        ft_putstr_fd("\033[0;32m→ ", STDERR);
    else if (mini->ret_value != 0)
        ft_putstr_fd("\033[0;31m→ ", STDERR);

    ft_putstr_fd("\033[1;33mminishell ▸ \033[0m", STDERR);
    fflush(stderr);
}