/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:28:41 by alex              #+#    #+#             */
/*   Updated: 2024/07/31 14:31:21 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include "lexer.h"
# include <stdio.h>
# include <string.h>

typedef enum
{
    NODE_PIPE 			= 1,
	NODE_REDIRECT_OUT	= 2,
    NODE_CMDPATH		= 3,
    NODE_ARGUMENT		= 4,
    NODE_DATA 			= 0,
} node_type;

typedef struct cmd_node {
	char		*data;
	int			type;
	struct cmd_node	*left;
	struct cmd_node	*right;
}	cmd_node;


void	    cmd_set(cmd_node *node, char *data, node_type nodetype, cmd_node *next);
void	    cmd_delete(cmd_node* node);
void	    cmd_attach(cmd_node* root, cmd_node* left, cmd_node* right);
void	    cmd_set_type(cmd_node* node, node_type nodetype);
void	    cmd_set_data(cmd_node* node, char* data);
bool        check_tokentype(int tok_type, t_token **token, char** bufferptr);
void        show_cmd_tree(cmd_node *node);

cmd_node    *job(t_token **token);
cmd_node	*job_pipe(t_token **token);
cmd_node	*cmd(t_token **token);
cmd_node	*cmd_redirect_out(t_token **token);
cmd_node	*cmd_simple(t_token **token);
cmd_node	*cmd_argument(t_token **token);
cmd_node	*argument(t_token **token);

cmd_node*	parser(t_token **token);

#endif