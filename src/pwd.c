// add header

#include "minishell.h"

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
