
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

//La idea seria pasarle un struct con todas las variables almacenadas que se hayan declarado con $ y que si las reconoce que
//las imprima. En cuanto este hecho export lo incluyo

void ft_echo(char *line) {
    char *arg;
    int new_line = 1;
    char *start, *end;

    arg = strtok(line, " \n");
    if (arg != NULL && ft_strcmp(arg, "-n") == 0)
	{
        new_line = 0;
        arg = strtok(NULL, " \n");
    }
    while (arg != NULL)
	{
        start = (arg[0] == '"') ? arg + 1 : arg;
        end = (arg[ft_strlen(arg) - 1] == '"') ? (arg + ft_strlen(arg) - 1) : NULL;

        if (end != NULL)
            write(1, start, end - start);
        else
            write(1, start, ft_strlen(start));
        arg = strtok(NULL, " \n");
        if (arg != NULL)
            write(1, " ", 1);
    }
    if (new_line)
        write(1, "\n", 1);
}
