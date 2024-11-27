/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:13:08 by mglikenf          #+#    #+#             */
/*   Updated: 2024/11/26 19:19:22 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO : TEST THIS FUNCTION FOR EXIT CODE 1 WITH MORE THAN ONE ARGUMENT, WHEN $? IS INTEGRATED

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
    ft_putstr_fd("minishell: exit: ", 2);
    ft_putstr_fd(cmd->args[1], 2);
    ft_putendl_fd(": numeric argument required", 2);
}

void    ft_exit(t_command *cmd, t_shell *shell)
{
    int n;

    n = 0;
    while (cmd->args[n + 1] != NULL)
        n++;
    ft_putendl_fd(cmd->name, 1);
    if (n == 1)
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
    exit(free_shell(shell));
}
