/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 20:36:13 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/21 12:18:14 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Global variable to store the signal number
 * volatile - the value of the variable can change at any time
 * sig_atomic_t - the type of the variable that can be accessed atomically
 */
volatile sig_atomic_t	g_signal = 0;

void	handle_signal(t_shell *shell)
{
	if (g_signal == SIGINT)
	{
		shell->exit_status = 130;
		g_signal = 0;
	}
}

/**
 * @brief Handle signals for executor
 */
void	exec_signals(int sig)
{
	if (sig == SIGQUIT)
		write(2, "Quit (core dumped)\n", 19);
	if (sig == SIGINT)
		write(2, "\n", 1);
}

static void	handle_sigint(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Handle EOF (ctrl-D)
 */
void	handle_eof(t_shell *shell)
{
	ft_putendl_fd("exit", 1);
	exit(free_shell(shell));
}

/**
 * @brief Handle signals for shell
 */
void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
