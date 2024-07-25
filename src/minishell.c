/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:05:22 by alex              #+#    #+#             */
/*   Updated: 2024/07/24 12:03:34 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer.h"
#include "parser.h"
#include "executor.h"

int main()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char **tokens;
    t_command *cmd;

    while (1)
    {
        ft_printf("> ");
        read = getline(&line, &len, stdin);
        if (read == -1) {
            perror("getline");
            exit(EXIT_FAILURE);
        }
        tokens = lexer(line);
        if (tokens[0] == NULL)
            continue;
        cmd = parser(tokens);
        executor(cmd);
        free(tokens);
        free(cmd);
    }
    free(line);
    return (0);
}