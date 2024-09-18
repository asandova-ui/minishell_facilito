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

typedef struct s_redirection {
    char *infile;
    char *outfile;
    char *heredoc_delim;
    int append_mode;
} t_redirection;


void init_redirection(t_redirection *red) {
    red->infile = NULL;
    red->outfile = NULL;
    red->heredoc_delim = NULL;
    red->append_mode = 0;
}

void free_redirection(t_redirection *red) {
    free(red->infile);
    free(red->outfile);
    free(red->heredoc_delim);
}

char *trim_whitespace(char *str) {
    while (*str && (*str == ' ' || *str == '\t'))
        str++;
    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
        end--;
    *(end + 1) = '\0';
    return str;
}

char **parse_command(char *cmd, t_redirection *red) {
    char **args = NULL;
    int arg_count = 0;
    char *token;
    char *rest = cmd;
    
    while ((token = strtok_r(rest, " \t", &rest))) {
        if (strcmp(token, "<") == 0 || strcmp(token, ">") == 0 ||
            strcmp(token, ">>") == 0 || strcmp(token, "<<") == 0) {
            char *file = strtok_r(NULL, " \t", &rest);
            if (file) {
                if (strcmp(token, "<") == 0) {
                    free(red->infile);
                    red->infile = strdup(file);
                } else if (strcmp(token, ">") == 0) {
                    free(red->outfile);
                    red->outfile = strdup(file);
                    red->append_mode = 0;
                } else if (strcmp(token, ">>") == 0) {
                    free(red->outfile);
                    red->outfile = strdup(file);
                    red->append_mode = 1;
                } else if (strcmp(token, "<<") == 0) {
                    free(red->heredoc_delim);
                    red->heredoc_delim = strdup(file);
                }
            }
        } else {
            args = realloc(args, (arg_count + 2) * sizeof(char *));
            args[arg_count++] = strdup(token);
        }
    }
    if (args)
        args[arg_count] = NULL;
    return args;
}

void handle_heredoc(t_redirection *red) {
    if (red->heredoc_delim) {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while (1) {
            write(STDOUT_FILENO, "> ", 2);
            read = getline(&line, &len, stdin);
            if (read == -1) break;
            line[strcspn(line, "\n")] = 0;

            if (strcmp(line, red->heredoc_delim) == 0) break;

            write(pipefd[1], line, strlen(line));
            write(pipefd[1], "\n", 1);
        }

        free(line);
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
    }
}

void apply_redirections(t_redirection *red) {
    if (red->infile) {
        int fd = open(red->infile, O_RDONLY);
        if (fd == -1) {
            perror("open infile");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (red->outfile) {
        int flags = O_WRONLY | O_CREAT;
        flags |= (red->append_mode) ? O_APPEND : O_TRUNC;
        int fd = open(red->outfile, flags, 0644);
        if (fd == -1) {
            perror("open outfile");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    handle_heredoc(red);
}

int execute_from_path(char **args, t_minish *mini) {
    if (!mini->path) {
        fprintf(stderr, "PATH is not set\n");
        return 0;
    }
    int i = 0;
    while (mini->path[i]) {
        char *exec_path = ft_strjoin_3args(mini->path[i], '/', args[0]);
        if (access(exec_path, X_OK) == 0) {
            execve(exec_path, args, mini->envp);
            perror("execve");
            free(exec_path);
            exit(EXIT_FAILURE);
        }
        free(exec_path);
        i++;
    }
    return 0;
}

void execute_command(char **args, t_minish *mini, t_redirection *red) {
    apply_redirections(red);

    if (args[0][0] == '/' || args[0][0] == '.') {
        // Absolute or relative path
        if (access(args[0], X_OK) == 0) {
            execve(args[0], args, mini->envp);
            perror("execve");
        } else {
            fprintf(stderr, "Command not found or not executable: %s\n", args[0]);
        }
    } else {
        // Search in PATH
        if (mini->path != NULL) {
            execute_from_path(args, mini);
        }
        fprintf(stderr, "Command not found: %s\n", args[0]);
    }
    exit(EXIT_FAILURE);
}

void execute_pipeline(char *line, t_minish *mini) {
    char **commands = ft_split(line, '|');
    int i = 0;
    int pipefd[2];
    int prev_pipe = -1;

    while (commands[i]) {
        t_redirection red;
        init_redirection(&red);
        
        char *trimmed_cmd = trim_whitespace(commands[i]);
        char **args = parse_command(trimmed_cmd, &red);

        if (commands[i + 1] && pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (prev_pipe != -1) {
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }
            if (commands[i + 1]) {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            execute_command(args, mini, &red);
        } else {
            if (prev_pipe != -1) close(prev_pipe);
            if (commands[i + 1]) {
                close(pipefd[1]);
                prev_pipe = pipefd[0];
            } else {
                close(pipefd[0]);
                close(pipefd[1]);
            }
            // Wait for the child process to finish
            int status;
            waitpid(pid, &status, 0);
        }

        free_redirection(&red);
        for (int j = 0; args[j]; j++)
            free(args[j]);
        free(args);
        i++;
    }

    // Close the last pipe if it's still open
    if (prev_pipe != -1) close(prev_pipe);

    for (i = 0; commands[i]; i++) {
        free(commands[i]);
    }
    free(commands);
}

int run_command(char *line, t_minish *mini) {
    int result = 1;
    char *trimmed_line = trim_whitespace(line);

    if (strchr(trimmed_line, '|') != NULL) {
        execute_pipeline(trimmed_line, mini);
    } else {
        t_redirection red;
        init_redirection(&red);
        char **args = parse_command(trimmed_line, &red);

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return result;
        } else if (pid == 0) {
            execute_command(args, mini, &red);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                result = 0;
            }
        }

        free_redirection(&red);
        for (int i = 0; args[i]; i++)
            free(args[i]);
        free(args);
    }

    return result;
}