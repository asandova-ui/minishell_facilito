/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:10:50 by alonso            #+#    #+#             */
/*   Updated: 2024/09/23 14:50:36 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

extern t_minish *g_mini;

void sig_handler(int signum)
{
    if (signum == SIGINT)
    {
        if (g_mini->pid == 0)
        {
            write(STDERR_FILENO, "\n", 1);
            rl_on_new_line();
            rl_replace_line("", 0);
            rl_redisplay();
        }
        else
        {
            kill(g_mini->pid, SIGINT);
            write(STDERR_FILENO, "\n", 1);
        }
        g_mini->ret_value = 130;
    }
    else if (signum == SIGQUIT)
    {
        if (g_mini->pid != 0)
        {
            write(STDERR_FILENO, "Quit: 3\n", 8);
            g_mini->ret_value = 131;
        }
        else
        {
            rl_on_new_line();
            rl_redisplay();
        }
    }
}

void setup_signals(void)
{
    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);
}

void reset_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}



/*void    sig_quit(int code)
{
    char    *nbr;

    nbr = ft_itoa(code);
    if (g_sig.pid != 0)
    {
        ft_putstr_fd("Quit: ", STDERR);
        ft_putendl_fd(nbr, STDERR);
        g_sig.exit_status = 131;
        g_sig.sigquit = 1;
        if (g_mini)
            g_mini->ret_value = 131;
    }
    else
        ft_putstr_fd("\b\b  \b\b", STDERR);
    free(nbr);
}

void    sig_int(int code)
{
    (void)code;
    if (g_sig.pid == 0)
    {
        ft_putstr_fd("\n", STDERR);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_sig.exit_status = 130;
        if (g_mini)
            g_mini->ret_value = 130;
    }
    else
    {
        ft_putstr_fd("\n", STDERR);
        g_sig.exit_status = 130;
        if (g_mini)
            g_mini->ret_value = 130;
    }
    g_sig.sigint = 1;
}*/