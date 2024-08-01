/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:05:22 by alex              #+#    #+#             */
/*   Updated: 2024/07/31 21:41:04 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include "executor.h"

int main()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char **tokens;
    cmd_node *cmd;
    t_token *token;

    while (1)
    {
        ft_printf("minishell$ ");
        read = getline(&line, &len, stdin);
        if (read == -1) {
            perror("getline");
            exit(EXIT_FAILURE);
        }
        tokens = lexer(line);
        token = convert_from_tokens(tokens);
        //print_tokens(token);
        cmd = parser(&token);
        //show_cmd_tree(cmd);
        //printf("\n_______\n");
        executor(cmd);
        free(tokens);
        free(token);
    }
    free(line);
    return (0);
}