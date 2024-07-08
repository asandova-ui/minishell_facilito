
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

int		ft_env(char **paths)
{
    int i;

    i = 0;
	while (paths[i] != '\0')
	{
		ft_putendl_fd(paths[i], 1);
		i++;
	}
	return (0);
}
