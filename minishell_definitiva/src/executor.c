/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:06:54 by alonso            #+#    #+#             */
/*   Updated: 2024/09/27 16:25:51 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../printf/includes/ft_printf.h"
#include "../printf/libft/libft.h"

void	init_command_context(t_command_context *ctx, char *line, t_minish *mini)
{
	ctx->line = line;
	ctx->trimmed_line = NULL;
	ctx->expanded_line = NULL;
	ctx->mini = mini;
}
void	handle_child_process(char **args, t_minish *mini, t_redirection *red)
{
	execute_command(args, mini, red);
	exit(EXIT_FAILURE);
}

void	handle_parent_process(pid_t pid, t_minish *mini)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		mini->ret_value = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->ret_value = 128 + WTERMSIG(status);
	else
		mini->ret_value = 1;
}

void	execute_single_command(t_command_context *ctx)
{
	t_redirection	red;
	char			**args;
	pid_t			pid;

	init_redirection(&red);
	args = parse_command(ctx->expanded_line, &red);
	pid = fork();
	if (pid == -1)
		ctx->mini->ret_value = 1;
	else if (pid == 0)
		handle_child_process(args, ctx->mini, &red);
	else
		handle_parent_process(pid, ctx->mini);
	free_redirection(&red);
	free_args(args);
}

char* parse_command_quotes(const char* command) {
    // Si el comando empieza con comillas simples o dobles, devolver tal cual
    if (command[0] == '\'' || command[0] == '"') {
        return strdup(command);  // Retorna una copia del comando original
    }

    int len = strlen(command);
    char* result = malloc(len + 1);
    int result_index = 0;
    bool in_single_quotes = false;
    bool in_double_quotes = false;
    bool last_char_was_space = true;
    bool is_echo_command = (strncmp(command, "/bin/echo ", 10) == 0);

    for (int i = 0; i < len; i++) {
        char c = command[i];

        if (c == '\'' && !in_double_quotes) {
            in_single_quotes = !in_single_quotes;
            if (is_echo_command && command[i + 1] == '$') {
                // Mantener comillas simples alrededor de variables para echo '$var'
                result[result_index++] = c;
            } else if (!in_single_quotes) {
                // Si es una comilla simple de cierre, agregarla al resultado
                result[result_index++] = c;
            }
        } else if (c == '"' && !in_single_quotes) {
            in_double_quotes = !in_double_quotes;
            if (!in_double_quotes && is_echo_command && command[i + 1] == '$') {
                // Eliminar comillas dobles alrededor de variables para echo "$var"
                continue;
            }
        } else if (c == ' ' && !in_single_quotes && !in_double_quotes) {
            if (!last_char_was_space) {
                result[result_index++] = c;
                last_char_was_space = true;
            }
        } else {
            result[result_index++] = c;
            last_char_was_space = false;
        }
    }

    if (result_index > 0 && result[result_index - 1] == ' ') {
        result_index--;
    }

    result[result_index] = '\0';
    return result;
}



int	run_command(char *line, t_minish *mini)
{
	t_command_context	ctx;
	char				*quoted_line;

	printf("LINEA ORIGINAL:           %s\n", line);

	quoted_line = handle_quotes(line);//esto me ayuda a manejar el quote>, por si no estan cerradas las llaves

	init_command_context(&ctx, quoted_line, mini);
	
	ctx.trimmed_line = trim_whitespace(line);
	printf("LINEA MODIF trimmed  :    %s\n", ctx.trimmed_line);
	
	ctx.trimmed_line = parse_command_quotes(ctx.trimmed_line);
	printf("LINEA MODIF QUOTES  :     %s\n", ctx.trimmed_line);

	ctx.expanded_line = expand_env_vars(ctx.trimmed_line, mini);
	printf("LINEA EXPANDIDA FINAL  :  %s\n", ctx.expanded_line);

	if (ft_strchr(ctx.expanded_line, '|'))
		execute_pipeline(ctx.expanded_line, ctx.mini);
	else
		execute_single_command(&ctx);
	cleanup_command_context(&ctx);
	free(quoted_line);
	return (mini->ret_value);
}
