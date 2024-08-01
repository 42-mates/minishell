/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:29:08 by alex              #+#    #+#             */
/*   Updated: 2024/07/31 14:48:52 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer.h"

/*
I try to parse the structure to make it working with pipes:

JOB :: { CMD | JOB } { CMD }
CMD :: { CMD_PATH ARGUMENT }

ASTreeNode* CMDLINE();		//	<job>

ASTreeNode* JOB();			// test all job production in order
ASTreeNode* JOB_PIPE();			// <command> '|' <job>
ASTreeNode* JOB_CMD();			// <command>

ASTreeNode* CMD();			// test all command production orderwise
ASTreeNode* CMD_SIMPLE();			//	<simple command>
*/

cmd_node*	job(t_token **token)
{
	t_token		*save;
    cmd_node	*node;

	save = *token;
	if ((*token = save, node = job_pipe(token)) != NULL)
        return node;
   	if ((*token = save, node = cmd(token)) != NULL)
        return node;
    return NULL;
}

cmd_node*	job_pipe(t_token **token)
{
	cmd_node* cmd_tok;
    cmd_node* job_tok;
    cmd_node* result;

    if ((cmd_tok = cmd(token)) == NULL)
        return (NULL);
	if (!check_tokentype(CHAR_PIPE, token, NULL))
	{
		cmd_delete(cmd_tok);
        return (NULL);
	}
    if ((job_tok = job(token)) == NULL)
	{
        cmd_delete(cmd_tok);
        return (NULL);
    }
    result = malloc(sizeof(cmd_node));
	cmd_set_type(result, NODE_PIPE);
    cmd_attach(result, cmd_tok, job_tok);
    return result;
}

cmd_node	*cmd(t_token **token)
{
	t_token		*save;
	cmd_node	*node;
	
	save = *token;
	if ((*token = save, node = cmd_redirect_out(token)) != NULL)
		return (node);
    if ((*token = save, node = cmd_simple(token)) != NULL)
        return (node);
    return (NULL);
}

cmd_node	*cmd_redirect_out(t_token **token)
{
	cmd_node* cmd_tok;
    cmd_node* result;

    if ((cmd_tok = cmd_simple(token)) == NULL)
        return NULL;
	if (!check_tokentype(CHAR_GREATER, token, NULL)) {
		cmd_delete(cmd_tok);
		return NULL;
	}
	char* filename;
	if (!check_tokentype(TOKEN, token, &filename)) {
		free(filename);
		cmd_delete(cmd_tok);
		return NULL;
	}
	//printf("filename: %s\n", filename);
    result = malloc(sizeof(*result));
    cmd_set_type(result, NODE_REDIRECT_OUT);
    cmd_set_data(result, filename);
	cmd_attach(result, NULL, cmd_tok);
    return result;
}

cmd_node	*cmd_simple(t_token **token)
{
	cmd_node	*arg;
    cmd_node	*result;
	char		*res;

    if (!check_tokentype(TOKEN, token, &res))
        return (NULL);
    arg = cmd_argument(token);
    result = malloc(sizeof(cmd_node));
	cmd_set_type(result, NODE_CMDPATH);
    cmd_set_data(result, res);
	cmd_attach(result, NULL, arg);
    return result;
}

cmd_node	*cmd_argument(t_token **token)
{
	t_token		*save;
    cmd_node	*node;
	
	save = *token;
    if ((*token = save, node = argument(token)) != NULL)
        return (node);
    if ((*token = save, node = NULL) != NULL)
        return (node);
    return (NULL);
}

cmd_node	*argument(t_token **token)
{
    cmd_node	*arg;
    cmd_node	*result;
	char		*res;

    if (!check_tokentype(TOKEN, token, &res))
        return (NULL);
	arg = cmd_argument(token);
    result = malloc(sizeof(cmd_node));
    cmd_set_type(result, NODE_ARGUMENT);
    cmd_set_data(result, res);
	cmd_attach(result, NULL, arg);
    return (result);
}

cmd_node*	parser(t_token **token)
{
	cmd_node	*cmd_tree;

	cmd_tree = job(token);
	return (cmd_tree);
}
