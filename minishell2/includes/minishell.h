/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:41:11 by asandova          #+#    #+#             */
/*   Updated: 2024/07/11 12:05:36 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../get_next_line/get_next_line.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2
# define MAX_EXIT_VALUE 255

# define MAX_INT 2147483647

typedef struct s_minish
{
	char			**path;
	char			**envp;
	char			**av;
	int				in;
	int				out;
	int				exit;
	int				ret_value;
	int				env_exist;
	int				comillas;
}					t_minish;

typedef struct s_token
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

typedef struct s_sig
{
	int				sigint;
	int				sigquit;
	int				exit_status;
	pid_t			pid;
}					t_sig;

void				init_envp(t_minish *mini, char *envp[]);
void				init_struct(t_minish *mini, char **envp);
void				minishell(char *line, t_minish *mini);
void				built_ins(char *line, t_minish *mini);
int					ft_strcmp(const char *str1, const char *str2);
void				exit_command(void);
void				ft_env(t_minish *mini);
int					ft_pwd(void);
void				print_line(t_minish *mini);
void				parse_line(char *line, t_minish *mini);
int					quotes(char *line, int index);
int					is_important_sep(char *line, int i);
void				ft_echo(char *line);
void				ft_export(char *args, t_minish *mini);
void				free_envp(char **envp);
char				**dup_envp(char **envp);
void				ft_unset(char *name, t_minish *mini);
void				remove_env_var(t_minish *mini, const char *name);
void				add_or_update_env_var(t_minish *mini, const char *name,
						const char *value);
int					is_valid_env(const char *env);
int					print_error(int error, const char *arg);
void				print_sorted_envp(t_minish *mini);
int					compare_env(const void *a, const void *b);
void				ft_qsort(void *base, size_t nitems, size_t size,
						int (*compar)(const void *, const void *));
void				ft_cd(char *path, t_minish *mini);
void				handle_sigint(int sig);
char				*get_env_value(const char *name);
char				*ft_strndup(const char *s, size_t n);
char				*ft_strncpy(char *dest, const char *src, size_t n);
void				sig_int(int code);
void				sig_init(void);
extern t_sig		g_sig;
void	ft_exit(char *args);
int	is_valid_number(const char *str);
#endif