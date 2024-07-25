/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:29:08 by alex              #+#    #+#             */
/*   Updated: 2024/07/24 11:52:48 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_command *parser(char **tokens)
{
	t_command	*cmd;
	
	cmd = malloc(sizeof(t_command));
	if (!cmd) {
		ft_printf("allocation error\n");
		exit(EXIT_FAILURE);
	}
	cmd->name = tokens[0];
	cmd->args = tokens;
	return cmd;
}
