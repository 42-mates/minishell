/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:42:50 by oprosvir          #+#    #+#             */
/*   Updated: 2024/10/29 02:23:05 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parser(char **tokens)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		ft_printf("allocation error\n");
		exit(EXIT_FAILURE);
	}
	cmd->name = tokens[0];
	cmd->args = tokens;
	return (cmd);
}
