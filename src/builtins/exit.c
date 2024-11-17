/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mglikenf <mglikenf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:13:08 by mglikenf          #+#    #+#             */
/*   Updated: 2024/11/15 17:56:11 by mglikenf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TEST THIS FUNCTION FOR EXIT CODE 1 WITH MORE THAN ONE ARGUMENT, WHEN $? IS INTEGRATED

char    *parse_exit_args(t_command *cmd, t_shell *shell)
{
    int i;
    int n_args;

    n_args = 1;
    while (cmd->args[n_args] != NULL)
        n_args++;
    if (n_args == 1)
        return (NULL);
    else if (n_args >= 2)
    {
        i = 0;
        while (cmd->args[1][i])
        {
            if (ft_isdigit(cmd->args[1][i]) != 1)
            {
                shell->exit_status = 2;
                return ("numeric argument required");
            }
            i++;
        }
    }
    if (cmd->args[2] != NULL)
    {
        shell->exit_status = 1;
        return ("too many arguments");
    }
    return (NULL);
}

int     first_arg_is_numeric(t_command *cmd)
{
    int     i;

    i = 0;
    while (cmd->args[1][i])
    {
        if (ft_isdigit(cmd->args[1][i]) != 1)
            return (-1);
        i++;
    }
    return (1);
}

void    builtin_syntax_error(t_command *cmd, t_shell *shell)
{
    shell->exit_status = 2;
    ft_putstr_fd("minishell: exit: ", 1);
    ft_putstr_fd(cmd->args[1], 1);
    ft_putendl_fd(": numeric argument required", 1);
}

void    ft_exit(t_command *cmd, t_shell *shell)
{
    int n;

    n = 0;
    while (cmd->args[n + 1] != NULL)
        n++;
    ft_putendl_fd(cmd->name, 1);
    if (n == 0)
        shell->exit_status = 0;
    else if (n == 1)
    {
        // check if also in range!
        if (first_arg_is_numeric(cmd) == 1)
            shell->exit_status = ft_atoi(cmd->args[1]);
        else
            builtin_syntax_error(cmd, shell);
    }
    else if (n >= 2)
    {
        if (first_arg_is_numeric(cmd) == 1)
        {
            // CHECK echo $? INSIDE SHELL WHEN INTEGRATED - EXIT STATUS SHOULD BE 1
            ft_putendl_fd("minishell: exit: too many arguments", 1);
            shell->exit_status = 1;
            return;
        }
        else
            builtin_syntax_error(cmd, shell);
    }
    free_command(cmd);
    // leak from array envp in parser
    exit(shell->exit_status);
}


// void	ft_exit(t_command *cmd, t_shell *shell)
// {
//     if (cmd->args[1] && cmd->args[2] == NULL)
//         shell->exit_status = ft_atoi(cmd->args[1]);
//     ft_putendl_fd(cmd->name, 1);
//     free_command(cmd);
// 	exit(free_shell(shell));
// }