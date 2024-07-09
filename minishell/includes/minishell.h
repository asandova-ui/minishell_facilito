/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:41:11 by asandova          #+#    #+#             */
/*   Updated: 2024/07/08 16:43:08 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <linux/limits.h>
# include <errno.h>
# include <signal.h>
# include "../get_next_line/get_next_line.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

typedef struct s_minish
{
    char **path;
    char **envp;
    char **av;
    int in;
    int out;
    int exit;
    int ret_value;
    int env_exist;
}   t_minish;

typedef struct	s_token
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;

void            init_envp(t_minish *mini, char *envp[]);
void            init_struct(t_minish *mini, char **envp);
void            minishell(char *line, t_minish	*mini);
void	        built_ins(char *line, t_minish	*mini);
int             ft_strcmp(const char *str1, const char *str2);
void	        exit_command(void);
int		        ft_env(char **paths);
int		        ft_pwd(void);
void            print_line(t_minish *mini);
void            parse_line(char *line, t_minish *mini);

#endif