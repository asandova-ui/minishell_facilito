/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:41:11 by asandova          #+#    #+#             */
/*   Updated: 2024/09/26 12:04:01 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../get_next_line/get_next_line.h"
# include <ctype.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define MAX_INT 2147483647

# define MAX_EXIT_VALUE 255

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
	int				exec;
	pid_t			pid;
	int				redisplay_prompt;
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

typedef struct s_redirection
{
	char			*infile;
	char			*outfile;
	char			*heredoc_delim;
	int				append_mode;
}					t_redirection;

typedef struct s_pipeline
{
	int				cmd_count;
	char			**commands;
	int				pipes[100][2];
	pid_t			pids[100];
}					t_pipeline;

typedef struct s_command_context
{
	char			*line;
	char			*trimmed_line;
	char			*expanded_line;
	t_minish		*mini;
}					t_command_context;

typedef struct s_env_var
{
	char			*name;
	char			*value;
	size_t			var_len;
	char			*r;
	char			*c;
	char			*new_r;
}					t_env_var;

extern t_minish		*g_mini;

typedef struct s_history
{
	char			*history[1000];
	int				count;
	int				i;
}					t_history;

typedef struct s_QuoteState
{
	char			*result;
	const char		*start;
	int				in_single_quote;
	int				in_double_quote;
}					t_quoteState;

typedef struct s_EchoState
{
	char			*processed_line;
	char			*arg;
	int				new_line;
	t_minish		*mini;
}					t_echoState;

typedef struct s_EchoQuoteState
{
	char			*current;
	int				in_single_quote;
	int				in_double_quote;
}					t_echoQuoteState;

typedef struct s_ExitState
{
	char			*args;
	int				num_args;
	int				exit_value;
}					t_exitState;

typedef struct s_ExportState
{
	char			*args;
	t_minish		*mini;
	int				ret_value;
}					t_exportState;

typedef struct s_EnvVarState
{
	t_minish		*mini;
	const char		*name;
	const char		*value;
}					t_envVarState;

typedef struct s_SortedEnvState
{
	char			**sorted_envp;
	int				env_count;
}					t_sortedEnvState;

typedef struct s_qsort_ctx
{
	void			*base;
	size_t			size;
	int				(*compar)(const void *, const void *);
}					t_qsort_ctx;

typedef struct s_cd_ctx
{
	char			cwd[PATH_MAX];
	char			*oldpwd;
	t_minish		*mini;
}					t_cd_ctx;

typedef struct s_cmd_ctx
{
	char			*line;
	char			*line_copy;
	char			*command;
	char			*args;
}					t_cmd_ctx;

typedef struct s_parse_data
{
	char			**args;
	int				arg_count;
	char			*token;
	char			*rest;
	char			*file;
}					t_parse_data;

typedef struct s_quote_state
{
	int				in_single_quote;
	int				in_double_quote;
}					t_quote_state;

void				init_path(t_minish *mini);
void				init_struct(t_minish *mini, char **envp);
int					minishell(t_minish *mini, t_history *history);
void				built_ins(char *line, t_minish *mini, t_history *history);
int					ft_strcmp(const char *str1, const char *str2);
void				exit_command(void);
int					ft_env(t_minish *mini);
int					ft_pwd(void);
void				parse_line(char *line, t_minish *mini);
int					quotes(char *line, int index);
int					is_important_sep(char *line, int i);
int					ft_echo(char *line, t_minish *mini);
int					ft_export(char *args, t_minish *mini);
void				free_envp(char **envp);
char				**dup_envp(char **envp);
int					ft_unset(char *args, t_minish *mini);
int					remove_env_var(t_minish *mini, const char *name);
int					add_or_update_env_var(t_minish *mini, const char *name,
						const char *value);
int					is_valid_env(const char *env);
int					print_error(int error, const char *arg);
int					print_sorted_envp(t_minish *mini);
int					compare_env(const void *a, const void *b);
void				ft_qsort(void *base, size_t nitems, size_t size,
						int (*compar)(const void *, const void *));
int					ft_cd(char *path, t_minish *mini);
int					run_command(char *line, t_minish *mini);
void				sig_int(int code);
// void				sig_init(void);
void				ft_exit(char *args);
char				*ft_strndup(const char *s, size_t n);
char				*ft_strncpy(char *dest, const char *src, size_t n);
char				*get_env_value(const char *name, t_minish *mini);
int					is_valid_number(const char *str);
void				sig_quit(int code);
void				init_struct_history(t_history *history);
void				add_to_history(t_history *history, char *command);
void				show_history(t_history *history, t_minish *mini);
bool				has_redirection(const char *str);
char				*redirect_echo(char *line);
void				executor(char *line, t_minish *mini);
void				free_paths(char **paths);
void				free_history(t_history *history);
void				setup_signals(void);
char				*ft_strjoin_3args(char const *s1, char connector,
						char const *s2);
char				*get_env_value(const char *name, t_minish *mini);
char				*handle_quotes(const char *str);

char				*expand_env_vars(char *str, t_minish *mini);
//char *expand_env_vars(const char *line, t_minish *mini);

char				*trim_whitespace(char *str);
char				**parse_command(char *cmd, t_redirection *red);
void				handle_heredoc(t_redirection *red);
void				apply_redirections(t_redirection *red);
void				free_redirection(t_redirection *red);
void				init_redirection(t_redirection *red);
void				execute_pipeline(char *line, t_minish *mini);
void				execute_command(char **args, t_minish *mini,
						t_redirection *red);
void				close_all_pipes(t_pipeline *pipeline);
void				free_envp(char **envp);
char				**dup_envp(char **envp);
char				*mini_getenv(t_minish *mini, const char *name);
void				process_quotes(t_quoteState *state, int is_append);
void				handle_incomplete_quotes(t_quoteState *state);
int					handle_empty_arg(t_echoState *state);
void				handle_n_flag(t_echoState *state);
void				process_variable(t_echoQuoteState *qstate,
						t_echoState *estate);
void				process_current_char(t_echoQuoteState *qstate);
void				process_arg(t_echoState *estate);
void				initialize_echo_state(t_echoState *state, char *line,
						t_minish *mini);
void				initialize_exit_state(t_exitState *state, char *args);
void				handle_empty_args(t_exitState *state);
int					count_args(char *args);
void				handle_too_many_args(t_exitState *state);
void				process_exit_arg(t_exitState *state);
int					print_export_sorted_envp(t_minish *mini);
int					is_valid_export_env(const char *env);
int					add_or_update_export_env_var(t_minish *mini,
						const char *name, const char *value);
int					compare_export_env(const void *a, const void *b);
int					print_export_error(int error, const char *arg);
void				init_export_env_var_state(t_envVarState *state,
						t_minish *mini, const char *name, const char *value);
void				handle_unset(t_cmd_ctx *ctx, t_minish *mini);
void				free_args(char **args);
void				init_command_context(t_command_context *ctx, char *line,
						t_minish *mini);
void				init_env_var(t_env_var *env_var);
void				cleanup_command_context(t_command_context *ctx);
void				update_quote_state(const char *str, t_quote_state *state);
size_t				calculate_new_length(const char *result,
						const char *extra_input, int is_first_input);
char				*allocate_new_result(char *result, const char *extra_input,
						int is_first_input);
void				append_input(char **result, const char *extra_input,
						int is_first_input);
char				*initialize_result(const char *str);
int					check_spaces_history(char *command);
#endif