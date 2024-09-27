/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:56:29 by alonso            #+#    #+#             */
/*   Updated: 2024/09/27 15:23:54 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

void write_heredoc_content(int pipefd[2], char *heredoc_delim, bool is_interactive)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while (1)
    {
        if (is_interactive)
            write(STDERR_FILENO, "> ", 2);
        
        read = getline(&line, &len, stdin);
        if (read == -1)
            break;
        
        line[strcspn(line, "\n")] = 0;
        if (ft_strcmp(line, heredoc_delim) == 0)
            break;
        
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
    }
    free(line);
}

void handle_heredoc(t_redirection *red)
{
    int pipefd[2];
    bool is_interactive;

    if (red->heredoc_delim)
    {
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        
        // Comprobar si la entrada estándar es un terminal
        is_interactive = isatty(STDIN_FILENO);
        
        write_heredoc_content(pipefd, red->heredoc_delim, is_interactive);
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
    }
}

void	apply_input_redirection(t_redirection *red)
{
	int	fd;

	if (red->infile)
	{
		fd = open(red->infile, O_RDONLY);
		if (fd == -1)
		{
			perror("open infile");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	apply_output_redirection(t_redirection *red)
{
	int	flags;
	int	fd;

	if (red->outfile)
	{
		flags = O_WRONLY | O_CREAT;
		if (red->append_mode)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(red->outfile, flags, 0644);
		if (fd == -1)
		{
			perror("open outfile");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	apply_redirections(t_redirection *red)
{
	apply_input_redirection(red);
	apply_output_redirection(red);
	handle_heredoc(red);
}
