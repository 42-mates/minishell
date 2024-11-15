// add header

#include "minishell.h"

void    ft_exit(t_command *cmd, t_shell *shell)
{
    if (cmd->args[1] && cmd->args[2] == NULL)
        shell->exit_status = ft_atoi(cmd->args[1]);
    ft_putendl_fd(cmd->name, 1);
    exit(shell->exit_status);
}
