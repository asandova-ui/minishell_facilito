/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:10:50 by alonso            #+#    #+#             */
/*   Updated: 2024/09/30 10:36:51 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

t_signal_status	g_signal_info = {0, 0};

void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal_info.signal_status = 130;
		if (g_signal_info.is_executing)
		{
			write(STDERR_FILENO, "\n", 1);
		}
		else
		{
			write(STDERR_FILENO, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	handle_signal_status_remix(t_minish *mini)
{
	if (g_signal_info.signal_status != 0)
	{
		mini->ret_value = g_signal_info.signal_status;
		g_signal_info.signal_status = 0;
	}
}
