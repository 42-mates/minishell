/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 20:36:13 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/07 08:56:36 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_signals(int sig)
{
	if (sig == SIGQUIT)
		write(2, "Quit (core dumped)\n", 19);
	if (sig == SIGINT)
		write(2, "\n", 1);	
}

// SIGINT (ctrl-C)
// TODO : exit_status = 130 (global?)
static void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// EOF (ctrl-D)
void	handle_eof(t_shell *shell)
{
	ft_putendl_fd("exit", 1);
	exit(free_shell(shell));
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
