/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:42:50 by oprosvir          #+#    #+#             */
/*   Updated: 2024/12/13 13:35:04 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*parse_command(t_token **tokens, t_shell *shell,
		t_command **current)
{
	if (!*current)
	{
		*current = init_command(shell);
		if (!*current)
			return (NULL);
	}
	if ((*tokens)->type == R_HEREDOC)
	{
		if (!parse_heredoc(tokens, *current, shell))
			return (NULL);
	}
	if (is_redirect((*tokens)->type))
	{
		if (!parse_redirects(tokens, *current, shell))
			return (NULL);
	}
	else
		parse_args(tokens, *current);
	return (*current);
}

static t_command	*build_command(t_token *tokens, t_shell *shell)
{
	t_command	*head;
	t_command	*current;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!parse_pipe(&tokens, &current, shell))
				return (free_commands(head), NULL);
		}
		else
		{
			current = parse_command(&tokens, shell, &current);
			if (!current)
				return (free_commands(head), NULL);
			if (!head)
				head = current;
		}
	}
	return (head);
}

t_command	*parser(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_command	*head;

	tokens = lexer(line, shell);
	if (!tokens)
		return (NULL);
	//print_tokens(tokens);
	head = build_command(tokens, shell);
	free_tokens(tokens);
	return (head);
}
