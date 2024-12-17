#include "minishell.h"

// TODO : delete before pushing to intra
// TODO : also delete all // comments && unnecessary comments
const char *redirect_type_to_string(t_token_type type)
{
    switch (type)
    {
        case R_INPUT: return "Input (<)";
        case R_OUTPUT: return "Output (>)";
        case R_APPEND: return "Append (>>)";
        case R_HEREDOC: return "Heredoc (<<)";
        default: return "Unknown";
    }
}

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
    printf("Redirects:\n");
    t_redirect *redirect = cmd->redirects;
    while (redirect)
    {
        printf("  Type: %s, Filename: %s\n", redirect_type_to_string(redirect->type),
            redirect->filename ? redirect->filename : "(NULL)");
        redirect = redirect->next;
    }
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
        case R_INPUT:
            return "R_INPUT";
        case R_OUTPUT:
            return "R_OUTPUT";
        case R_APPEND:
            return "R_APPEND";
        case R_HEREDOC:
            return "R_HEREDOC";
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

