#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"
char	*ft_strjoin_3args(char const *s1, char connector, char const *s2)
{
	char	*str;
	size_t	x;
	size_t	y;

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

void execute_from_path(char *line, t_minish *mini)
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
            if (execve(ubication, command, mini->envp) == -1)
            {
                perror("execve errorcillo");
                return ;
            }
        }
        i ++;
    }
	/*char *args[1024];
	char *token = strtok(line, " ");
	int i = 0;

	while (token != NULL) {
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;

	// Ejecutar el comando
	if (execve(args[0], args, mini->envp) == -1) {
		perror("execvp");
		exit(EXIT_FAILURE);
	}*/
}

void execute_command(char *line, t_minish *mini)
{
    char **command;

    command = ft_split(line, ' ');
    if (access(command[0], X_OK) == 0)
    {
        if (execve(command[0], command, mini->envp))
        {
            perror("execve errorcillo");
            return ;
        }
    }
}

void handle_redirections(char *cmd) {
	char *infile = NULL;
	char *outfile = NULL;
	char *ptr;

	if ((ptr = strchr(cmd, '<')) != NULL) {
		*ptr = '\0';
		infile = strtok(ptr + 1, " ");
	}

	if ((ptr = strchr(cmd, '>')) != NULL) {
		*ptr = '\0';
		outfile = strtok(ptr + 1, " ");
	}

	if (infile) {
		int fd = open(infile, O_RDONLY);
		if (fd == -1) {
			perror("open infile");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}

	if (outfile) {
		int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1) {
			perror("open outfile");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void execute_pipeline(char *line, t_minish *mini) {
	char *commands[1024];
	char *token = strtok(line, "|");
	int i = 0;

	while (token != NULL) {
		commands[i++] = token;
		token = strtok(NULL, "|");
	}
	commands[i] = NULL;

	int num_commands = i;
	int pipe_fds[2 * (num_commands - 1)];

	for (i = 0; i < num_commands - 1; i++) {
		if (pipe(pipe_fds + i * 2) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}

	int pid;
	int status;

	for (i = 0; i < num_commands; i++) {
		pid = fork();
		if (pid == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			if (i > 0) {
				dup2(pipe_fds[(i - 1) * 2], STDIN_FILENO);
			}
			if (i < num_commands - 1) {
				dup2(pipe_fds[i * 2 + 1], STDOUT_FILENO);
			}

			for (int j = 0; j < 2 * (num_commands - 1); j++) {
				close(pipe_fds[j]);
			}

			handle_redirections(commands[i]);
			execute_command(commands[i], mini);
		}
	}

	for (i = 0; i < 2 * (num_commands - 1); i++) {
		close(pipe_fds[i]);
	}

	for (i = 0; i < num_commands; i++) {
		wait(&status);
	}
}

void run_command(char *line, t_minish *mini)
{
    int len = ft_strlen2(line);
    line[len - 1] = '\0';
	if (strchr(line, '|') != NULL) {
		execute_pipeline(line, mini);
        return;
	} else {
		int pid = fork();
		if (pid == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			handle_redirections(line);
            if (mini->path != NULL)
                execute_from_path(line, mini);
            else if (mini->path == NULL)
                execute_command(line, mini);
		} 
        else {
			int status;
			waitpid(pid, &status, 0);
		}
	}
}