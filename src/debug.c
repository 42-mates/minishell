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
        for (i = 0; cmd->args[i]; i++)
        {
            printf("  args[%d]: %s\n", i, cmd->args[i]);
        }
    }
    else
    {
        printf("  (No arguments)\n");
    }
    printf("Input file: %s\n", cmd->input_file ? cmd->input_file : "(NULL)");
    printf("Output file: %s\n", cmd->output_file ? cmd->output_file : "(NULL)");
    printf("Append file: %s\n", cmd->append_file ? cmd->append_file : "(NULL)");
    printf("Delimiter: %s\n", cmd->delimiter ? cmd->delimiter : "(NULL)");
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

const char *token_type_to_string(t_token_type type)
{
    switch (type)
    {
        case WORD:
            return "WORD";
        case PIPE:
            return "PIPE";
        case REDIRECT_IN:
            return "REDIRECT_IN";
        case REDIRECT_OUT:
            return "REDIRECT_OUT";
        case APPEND:
            return "APPEND";
        case HEREDOC:
            return "HEREDOC";
        default:
            return "UNKNOWN";
    }
}

void print_tokens(t_token *tokens)
{
    t_token *current = tokens;
    int index = 0;

    while (current)
    {
        printf("Token %d:\n", index);
        printf("  Type: %s\n", token_type_to_string(current->type));
        printf("  Value: '%s'\n", current->value);
        current = current->next;
        index++;
    }
}

