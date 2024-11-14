#include "minishell.h"

void	print_command(t_command *cmd)
{
	int	i;

	if (!cmd)
	{
		printf("Command is NULL\n");
		return ;
	}
	printf("Command name: %s\n", cmd->name);
	printf("Arguments:\n");
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			printf("  args[%d]: %s\n", i, cmd->args[i]);
			i++;
		}
	}
	else
		printf("  No arguments\n");
}
