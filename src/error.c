/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 20:06:11 by oprosvir          #+#    #+#             */
/*   Updated: 2024/11/26 20:55:22 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*err_msg(const char *msg, t_shell *shell, int exit_status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(msg, 2);
	shell->exit_status = exit_status;
	return (NULL);
}

// TODO : rewrite to use
void	error_exit(char *msg)
{
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	exit(EXIT_FAILURE);
}
