#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

char *ft_strjoin_3args(char const *s1, char connector, char const *s2)
{
    char *str;
    size_t x;
    size_t y;

    if (!s1 || !s2)
        return (NULL);
    str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 2));
    if (!str)
        return (NULL);
    x = 0;
    y = 0;
    while (s1[y])
        str[x++] = s1[y++];
    str[x++] = connector;
    y = 0;
    while (s2[y])
        str[x++] = s2[y++];
    str[x] = '\0';
    return (str);
}

int execute_from_path(char *line, t_minish *mini)
{
    int i;
    char **command;
    char *ubication;

    i = 0;
    command = ft_split(line, ' ');
    while (mini->path[i])
    {
        ubication = ft_strjoin_3args(mini->path[i], '/', command[0]);
        if (access(ubication, X_OK) == 0)
        {
            execve(ubication, command, mini->envp);
            perror("execve error");
            free(ubication);
            exit(EXIT_FAILURE); // Termina el proceso hijo si execve falla
        }
        free(ubication);
        i++;
    }

    // Si no encuentra un ejecutable en el path, retorna 0 para intentar con ruta absoluta
    return 0;
}

int execute_command(char *line, t_minish *mini)
{
    char **command;

    command = ft_split(line, ' ');
    if (access(command[0], X_OK) == 0)
    {
        execve(command[0], command, mini->envp);
        perror("execve error");
        free(command);
        return 1; // Error ejecutando el comando
    }
    fprintf(stderr, "Command not found or not executable: %s\n", command[0]);
    free(command);
    return 1; // Comando no encontrado o no ejecutable
}

void handle_redirections(char *cmd)
{
    char *infile = NULL;
    char *outfile = NULL;
    char *heredoc_delim = NULL;
    char *ptr;

    // Manejo de la redirección de entrada heredoc "<<"
    if ((ptr = strstr(cmd, "<<")) != NULL)
    {
        *ptr = '\0';
        heredoc_delim = strtok(ptr + 2, " \t");
    }

    // Manejo de la redirección de entrada "<"
    if ((ptr = strchr(cmd, '<')) != NULL && !heredoc_delim)
    {
        *ptr = '\0';
        infile = strtok(ptr + 1, " \t");
    }

    // Manejo de la redirección de salida ">>" o ">"
    if ((ptr = strstr(cmd, ">>")) != NULL)
    {
        *ptr = '\0';
        outfile = strtok(ptr + 2, " \t");
    }
    else if ((ptr = strchr(cmd, '>')) != NULL)
    {
        *ptr = '\0';
        outfile = strtok(ptr + 1, " \t");
    }

    // Manejo de heredoc "<<"
    if (heredoc_delim)
    {
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        char *line = NULL;
        size_t len = 0;

        // Leemos de la entrada hasta que se encuentre el delimitador
        while (1)
        {
            write(1, "> ", 2);
            getline(&line, &len, stdin);
            line[strlen(line) - 1] = '\0';

            if (strcmp(line, heredoc_delim) == 0)
                break;

            write(pipefd[1], line, strlen(line));
            write(pipefd[1], "\n", 1);
        }
        close(pipefd[1]);
        free(line);

        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
    }

    // Redirección de entrada "<"
    if (infile)
    {
        int fd = open(infile, O_RDONLY);
        if (fd == -1)
        {
            perror("open infile");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    // Redirección de salida ">>" o ">"
    if (outfile)
    {
        int fd;
        if (strstr(cmd, ">>") != NULL)
        {
            fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (fd == -1)
        {
            perror("open outfile");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

void execute_pipeline(char *line, t_minish *mini)
{
    char **commands = ft_split(line, '|');
    int i = 0;
    int pipefd[2];
    int prev_fd = -1;

    while (commands[i])
    {
        if (commands[i + 1])
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        int pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (commands[i + 1])
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            close(pipefd[0]);

            handle_redirections(commands[i]);
            if (mini->path != NULL && execute_from_path(commands[i], mini) == 0)
            {
                exit(EXIT_FAILURE);
            }
            else
            {
                execute_command(commands[i], mini);
            }
        }
        else
        {
            if (commands[i + 1])
            {
                close(pipefd[1]);
                prev_fd = pipefd[0];
            }
            else
            {
                if (prev_fd != -1)
                {
                    close(prev_fd);
                }
            }
            waitpid(pid, NULL, 0);
        }
        i++;
    }

    for (i = 0; commands[i]; i++)
    {
        free(commands[i]);
    }
    free(commands);
}

int run_command(char *line, t_minish *mini)
{
    int len = ft_strlen(line);
    line[len] = '\0';
    int result = 1;

    if (strchr(line, '|') != NULL)
    {
        execute_pipeline(line, mini);
    }
    else
    {
        int pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return result;
        }
        else if (pid == 0)
        {
            handle_redirections(line);

            if (mini->path != NULL && execute_from_path(line, mini) == 0)
            {
                exit(EXIT_FAILURE);
            }
            else
            {
                execute_command(line, mini);
            }
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            {
                result = 0;
            }
        }
    }

    return result;
}
