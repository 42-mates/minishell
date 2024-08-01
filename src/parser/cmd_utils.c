/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 11:06:35 by alex              #+#    #+#             */
/*   Updated: 2024/07/31 14:36:43 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool check_tokentype(int tok_type, t_token **token, char** bufferptr)
{
	if (!(*token))
		return (false);
    if ((*token)->type == tok_type)
    {
		if (bufferptr != NULL) {
			*bufferptr = malloc(strlen((*token)->data) + 1);
			strcpy(*bufferptr, (*token)->data);
		}
		*token = (*token)->next;
        return (true);
    }
    *token = (*token)->next;
    return (false);
}

void show_cmd_tree(cmd_node *node)
{
	if (node == NULL)
		return;
	if (node->type == NODE_CMDPATH)
	{
		printf("cmd: %s\n", node->data);
	}
	else if (node->type == NODE_ARGUMENT)
	{
		printf("arg: %s\n", node->data);
	}
	else if (node->type == NODE_PIPE)
	{
		printf("pipe: %s | %s\n", node->left->data, node->right->data);
	}
	else if (node->type == NODE_REDIRECT_OUT)
	{
		printf("redirect: %s > %s\n", node->right->data, node->data);
	}
	show_cmd_tree(node->right);
}

void cmd_attach(cmd_node* root, cmd_node* left, cmd_node* right)
{
	if (root == NULL)
		return ;
    root->left = left;
    root->right = right;
}

void cmd_set_type(cmd_node* node, node_type type)
{
    if (node == NULL)
		return ;
    node->type = type;
}

void cmd_set_data(cmd_node* node, char* data)
{
    if(data != NULL)
	{
        node->data = data;
    }
}

void	cmd_set(cmd_node *node, char *data, node_type type, cmd_node *next)
{
	if (node == NULL)
		return ;
	if(data != NULL)
	{
		node->data = data;
		node->type = type;
		node->left = next;
	}
}

void	cmd_delete(cmd_node *node)
{
	if (node == NULL)
		return;
	if (node->type >= 0)
		free(node->data);
	cmd_delete(node->left);
    cmd_delete(node->right);
    free(node);
}