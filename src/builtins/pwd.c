/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oprosvir <oprosvir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:12:56 by mglikenf          #+#    #+#             */
/*   Updated: 2024/11/27 11:49:56 by oprosvir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO : add pwd & oldpwd in shell struct
// error man : no PWD
void    ft_pwd(t_shell *shell)
{
    char    cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
        shell->exit_status = 0;
    }
    else
    {
        perror("pwd");
        shell->exit_status = 1;
    }
}
