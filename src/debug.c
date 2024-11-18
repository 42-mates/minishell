#include "minishell.h"

void print_command(t_command *cmd)
{
    int i;

    if (!cmd)
    {
        printf("Command is NULL\n");
        return;
    }

    printf("Command name: %s\n", cmd->name ? cmd->name : "NULL");

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
    {
        printf("  No arguments\n");
    }

    printf("Input file: %s\n", cmd->input_file ? cmd->input_file : "NULL");
    printf("Output file: %s\n", cmd->output_file ? cmd->output_file : "NULL");

    if (cmd->next)
    {
        printf("\n--- Next command ---\n");
        print_command(cmd->next);
    }
    else
    {
        printf("\nNo next command.\n");
    }
}

