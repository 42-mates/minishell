#include "minishell.h"

// printing environment variables from minishell
void	print_env_list(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		ft_putstr_fd(current->name, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(current->value, 1);
		ft_putstr_fd("\n", 1);
		current = current->next;
	}
}

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
