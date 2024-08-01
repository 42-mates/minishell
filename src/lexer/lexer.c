/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 10:43:03 by alex              #+#    #+#             */
/*   Updated: 2024/07/31 14:25:40 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	**lexer(char *line)
{
	return (ft_strtok(line, TOKEN_DELIM));
}

t_token *convert_from_tokens(char **tokens)
{
    t_token *head = NULL;
    t_token *prev = NULL;

    for (int i = 0; tokens[i] != NULL; i++)
    {
        t_token *token = malloc(sizeof(t_token));
        token->data = tokens[i];
		if (tokens[i][0] == '|')
        	token->type = CHAR_PIPE;
        else if (tokens[i][0] == '>')
        	token->type = CHAR_GREATER;
		else
			token->type = TOKEN;
        token->next = NULL;
        if (prev == NULL)
            head = token;
        else
            prev->next = token;
        prev = token;
    }

    return head;
}

void print_tokens(t_token *token)
{
    while (token)
    {
        ft_printf("data: %s\n", token->data);
        ft_printf("type: %d\n", token->type);
        token = token->next;
    }
}