// add header

#include "minishell.h"

void    ft_echo(t_command *cmd, t_shell *shell)
{
    int i;

    if (cmd->args[1] == NULL)
        ft_putendl_fd("", 1);
    else
    {
        i = 1;
        if (ft_strcmp(cmd->args[i], "-n") == 0)
            i++;
        while (cmd->args[i] != NULL)
        {
            ft_putstr_fd(cmd->args[i], 1);
            ft_putstr_fd(" ", 1);
            i++;
        }
        if (ft_strcmp(cmd->args[1], "-n") != 0)
            ft_putendl_fd("", 1);
    }
    shell->exit_status = 0;
}
