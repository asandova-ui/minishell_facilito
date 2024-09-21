
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void built_ins(char *line, t_minish *mini, t_history *history)
{
    char *command;
    char *args;
    int error;
    char *home;
    char *line_copy = strdup(line);

    if (line_copy == NULL) {
        mini->ret_value = 1;
        mini->exec = 1;
        return;
    }

    command = strtok(line, " \n");
    if (command == NULL)
    {
        mini->ret_value = 0;
        mini->exec = 1;
        free(line_copy);
        return;
    }

    if (ft_strcmp(command, "echo") == 0)
    {
        args = strtok(NULL, "\n");
        if (args == NULL)
        {
            write(1, "\n", 1);
            mini->ret_value = 0;
        }
        else if (has_redirection(args))
        {
            char *nueva_linea = redirect_echo(line_copy);
            if (nueva_linea != NULL)
            {
                mini->ret_value = run_command(nueva_linea, mini);
                free(nueva_linea);
            }
        }
        else
        {
            if (mini->comillas != 1)
            {
                ft_echo(args, mini);
            }
            mini->ret_value = 0;
        }
        mini->exec = 1;
    }
    else if (ft_strcmp(command, "cd") == 0)
    {
        args = strtok(NULL, "\n");
        if (args == NULL)
        {
            home = getenv("HOME");
            mini->ret_value = ft_cd(home, mini);
        }
        else
            mini->ret_value = ft_cd(args, mini);
        mini->exec = 1;
    }
    else if (ft_strcmp(command, "pwd") == 0)
    {
        mini->ret_value = ft_pwd();
        mini->exec = 1;
    }
    else if (ft_strcmp(command, "export") == 0)
    {
        args = strtok(NULL, "\n");
        if (args == NULL)
        {
            mini->ret_value = ft_export(NULL, mini);
        }
        else
        {
            error = is_valid_env(args);
            if (args[0] == '=')
                error = -3;
            if (error != 1)
            {
                print_error(error, args);
                mini->ret_value = 1;
            }
            else
                mini->ret_value = ft_export(args, mini);
        }
        mini->exec = 1;
    }
    else if (ft_strcmp(command, "unset") == 0)
    {
        args = strtok(NULL, "\n");
        if (args != NULL)
            mini->ret_value = ft_unset(args, mini);
        else
            mini->ret_value = 0;
        mini->exec = 1;
    }
    else if (ft_strcmp(command, "env") == 0)
    {
        mini->ret_value = ft_env(mini);
        mini->exec = 1;
    }
    else if (ft_strcmp(command, "exit") == 0)
    {
        args = strtok(NULL, "\n");
        ft_exit(args);  // Note: ft_exit doesn't return
        mini->exec = 1;
    }
    else if (ft_strcmp(command, "history") == 0)
    {
        show_history(history);
        mini->ret_value = 0;
        mini->exec = 1;
    }
    else
    {
        mini->ret_value = 127;  // Command not found
        mini->exec = 0;
    }

    free(line_copy);
}

int	ft_strcmp(const char *str1, const char *str2)
{
	while (*str1 && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	return (*(unsigned char *)str1 - *(unsigned char *)str2);
}

void show_history(t_history *history)
{
    int i;

	i = 0;
    while (i < history->count)
    {
        ft_printf("%d %s\n", i + 1, history->history[i]);
        i++;
    }
}

bool has_redirection(const char *str)
{
    bool in_quotes = false;
    char quote_char = '\0';

    while (*str)
    {
        if (*str == '"' || *str == '\'')
        {
            if (quote_char == '\0')
            {
                quote_char = *str;
                in_quotes = true;
            }
            else if (quote_char == *str)
            {
                quote_char = '\0';
                in_quotes = false;
            }
        }
        else if ((*str == '>' || *str == '<') && !in_quotes)
        {
            return true;
        }
        str++;
    }
    return false;
}

char *redirect_echo(char *line)
{
    char *new_line = NULL;
    size_t len;
    char *echo_position;
    const char *bin_echo = "/bin/echo";
    //printf("%s\n", line);
    
    // Busca la posición de 'echo' en la línea
    echo_position = strstr(line, "echo");
    if (echo_position == NULL)
    {
        return NULL; // No es un comando echo, no hacemos nada
    }

    // Verifica si hay redirección en la línea
    if (has_redirection(line))
    {
        // Calcula el tamaño de la nueva línea
        len = strlen(line) + strlen(bin_echo) - strlen("echo") + 1; // +1 para el terminador nulo
        new_line = (char *)malloc(len);
        if (new_line == NULL)
        {
            perror("malloc");
            return NULL; // Return NULL instead of exiting
        }

        // Construye la nueva línea
        strncpy(new_line, line, echo_position - line);
        new_line[echo_position - line] = '\0'; // Null-terminate after strncpy
        strcat(new_line, bin_echo);
        strcat(new_line, echo_position + strlen("echo"));

        return new_line;
    }

    return NULL; // No redirection, return NULL
}