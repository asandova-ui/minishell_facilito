#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

int		is_important_sep(char *line, int i)
{
	if (i > 0 && line[i - 1] == '\\' && ft_strchr("<>|;", line[i]))
		return (0);
	else if (ft_strchr("<>|;", line[i]) && quotes(line, i) == 0)
		return (1);
	else
		return (0);
}

char	*space_line(char *line)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
    new = (char *)malloc(sizeof(char));
	//new = space_alloc(line);
	while (new && line[i])
	{
		if (quotes(line, i) != 2 && line[i] == '$' && i && line[i - 1] != '\\')
			new[j++] = (char)(-line[i++]);
		else if (quotes(line, i) == 0 && is_important_sep(line, i))
		{
			new[j++] = ' ';
			new[j++] = line[i++];
			if (quotes(line, i) == 0 && line[i] == '>')
				new[j++] = line[i++];
			new[j++] = ' ';
		}
		else
			new[j++] = line[i++];
	}
	new[j] = '\0';
	return (new);
}

int		quotes(char *line, int index)
{
	int	i;
	int	open;
    int j;

	i = 0;
	open = 0;
	while (line[i] && i != index)
	{
		if (i > 0 && line[i - 1] == '\\')
			(void)j;
		else if (open == 0 && line[i] == '\"')
			open = 1;
		else if (open == 0 && line[i] == '\'')
			open = 2;
		else if (open == 1 && line[i] == '\"')
			open = 0;
		else if (open == 2 && line[i] == '\'')
			open = 0;
		i++;
	}
	return (open);
}

int quote_check(char *line, t_minish *mini)
{
    if (quotes(line, MAX_INT))
    {
        ft_putendl_fd("minishell: syntax error with open quotes", STDERR);
		mini->ret_value = 2;
		return (1);
    }
    return (0);
}

void parse_line(char *line, t_minish *mini)
{
    if (quote_check(line,mini))
        return ;
    line = space_line(line);//para tener en cuenta los caracteres especiales depende de si estamos dentro de comillas o no
    //depurar:
    //printf("%s\n", line);

    if (line && line[0] == '$')//si tenemos dolar por ejemplo $PATH lo convertimos de ASCII 36 a -36 para indicar que es "especial" y que hay que tratarlo de manera diferente
		line[0] = (char)(-line[0]);
    //depurar:
    //printf("%s\n", line);

    return ;
}