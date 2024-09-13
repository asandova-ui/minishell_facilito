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

int execute_from_path(char *line, t_minish *mini)
{
    int i;
    char **command;
    char *ubication;

    i = 0;
    command = ft_split(line, ' ');
    //write(1, "Intentando ejecutar desde PATH\n", 31);
    
    while (mini->path[i])
    {
        ubication = ft_strjoin_3args(mini->path[i], '/', command[0]);
        if (access(ubication, X_OK) == 0)
        {
            if (execve(ubication, command, mini->envp) == -1)
            {
                perror("execve errorcillo");
                exit(EXIT_FAILURE);  // Termina el proceso hijo si execve falla
            }
        }
        i++;
    }
    
    // Si no encuentra un ejecutable en el path, retorna 0 para intentar con ruta absoluta
    return 0;
}

int execute_command(char *line, t_minish *mini)
{
    char **command;

    //write(1, "Intentando ejecutar como ruta absoluta\n", 39);
    command = ft_split(line, ' ');
    if (access(command[0], X_OK) == 0)
    {
        if (execve(command[0], command, mini->envp) == -1)
        {
            perror("execve errorcillo");
            exit(EXIT_FAILURE);  // Termina el proceso hijo si execve falla
        }
    }
    // Si el comando no es accesible, imprime el error y retorna un fallo
    fprintf(stderr, "Command not found or not executable: %s\n", command[0]);
    return 1;  // Retornamos 1 si el comando no se encontró o no se pudo ejecutar
}

void handle_redirections(char *cmd) {
	char *infile = NULL;
	char *outfile = NULL;
	char *heredoc_delim = NULL;
	char *ptr;

	// Manejo de la redirección de entrada con "<"
	if ((ptr = strstr(cmd, "<<")) != NULL) {
		*ptr = '\0';  // Terminamos el comando en el primer "<"
		heredoc_delim = strtok(ptr + 2, " ");  // Obtenemos el delimitador
	}

	if ((ptr = strchr(cmd, '<')) != NULL && !heredoc_delim) {
		*ptr = '\0';  // Terminamos el comando en el primer "<"
		infile = strtok(ptr + 1, " ");  // Obtenemos el archivo de entrada
	}

	// Manejo de la redirección de salida con ">>" o ">"
	if ((ptr = strstr(cmd, ">>")) != NULL) {
		*ptr = '\0';  // Terminamos el comando en el primer ">"
		outfile = strtok(ptr + 2, " ");  // Obtenemos el archivo de salida
	} else if ((ptr = strchr(cmd, '>')) != NULL) {
		*ptr = '\0';  // Terminamos el comando en el primer ">"
		outfile = strtok(ptr + 1, " ");  // Obtenemos el archivo de salida
	}

	// Manejo de heredoc "<<"
	if (heredoc_delim) {
		int pipefd[2];
		if (pipe(pipefd) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		char *line = NULL;
		size_t len = 0;

		// Leemos de la entrada hasta que se encuentre el delimitador
		while (1) {
			write(1, "> ", 2);  // Muestra un prompt tipo heredoc
			getline(&line, &len, stdin);
			line[strlen(line) - 1] = '\0';  // Removemos el salto de línea

			if (strcmp(line, heredoc_delim) == 0)
				break;  // Terminamos si encontramos el delimitador

			write(pipefd[1], line, strlen(line));
			write(pipefd[1], "\n", 1);  // Añadimos la nueva línea
		}
		close(pipefd[1]);  // Cerramos el lado de escritura
		free(line);

		// Reemplazamos la entrada estándar por el lado de lectura de la tubería
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}

	// Si se especifica un archivo de entrada "<"
	if (infile) {
		int fd = open(infile, O_RDONLY);
		if (fd == -1) {
			perror("open infile");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDIN_FILENO);  // Reemplazamos la entrada estándar
		close(fd);
	}

	// Si se especifica un archivo de salida ">>" (append) o ">"
	if (outfile) {
		int fd;
		if (strstr(cmd, ">>") != NULL) {
			fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);  // Modo append
		} else {
			fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);  // Modo truncar
		}
		if (fd == -1) {
			perror("open outfile");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDOUT_FILENO);  // Reemplazamos la salida estándar
		close(fd);
	}
}

void execute_pipeline(char *line, t_minish *mini) {
    char **commands = ft_split(line, '|');
    int i = 0;
    int pipefd[2];
    int prev_fd = -1;

    while (commands[i]) {
        // Si no es el último comando, se necesita un pipe
        if (commands[i + 1]) {
            if (pipe(pipefd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        int pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Proceso hijo
            // Redirigir la entrada estándar si no es el primer comando
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            // Redirigir la salida estándar si no es el último comando
            if (commands[i + 1]) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            // Cerrar los descriptores de archivo del pipe
            close(pipefd[0]);

            // Ejecutar el comando
            handle_redirections(commands[i]);
            if (mini->path != NULL) {
                execute_from_path(commands[i], mini);
            } else {
                execute_command(commands[i], mini);
            }
            
            // Si llegamos aquí, algo salió mal
            exit(EXIT_FAILURE);
        } else {
            // Proceso padre
            // Cerrar los descriptores de archivo del pipe
            if (commands[i + 1]) {
                close(pipefd[1]);
                prev_fd = pipefd[0];
            }
            waitpid(pid, NULL, 0);
        }
        i++;
    }

    // Cerrar el descriptor de archivo final si existe
    if (prev_fd != -1) {
        close(prev_fd);
    }

    // Liberar memoria
    for (i = 0; commands[i]; i++) {
        free(commands[i]);
    }
    free(commands);
}


int run_command(char *line, t_minish *mini)
{
    int len = ft_strlen2(line);
    line[len - 1] = '\0';
    int result = 1; // Inicialmente asumimos que hubo un fallo

    if (strchr(line, '|') != NULL) {
        execute_pipeline(line, mini);
    } else {
        int pid = fork();
        if (pid == -1) {
            perror("fork");
            return result;  // Fallo al hacer fork
        } else if (pid == 0) {
            // Proceso hijo
            handle_redirections(line);
            
            // Primero intenta ejecutar usando mini->path
            if (mini->path != NULL && execute_from_path(line, mini) == 0)
            {
                // Si no encuentra el comando en el PATH, intenta ejecutarlo como ruta absoluta
                if (execute_command(line, mini) == 1) {
                    exit(EXIT_FAILURE);  // Si falla, el proceso hijo termina
                }
            }

            // Si llegamos aquí, algo salió mal, así que debemos terminar el proceso hijo
            exit(EXIT_FAILURE);
        } else {
            // Proceso padre
            int status;
            waitpid(pid, &status, 0);
            
            // Si el hijo terminó normalmente y no hubo errores en el comando
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                result = 0;  // Comando ejecutado con éxito
            }
        }
    }

    return result;  // Retorna 0 si tuvo éxito, 1 si falló
}
/*void execute_pipeline(char *line, t_minish *mini) {
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
}*/
