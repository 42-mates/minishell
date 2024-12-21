# minishell
```
FINAL GRADE: 101/100
```
Made by [@prosviriakova](https://github.com/prosviriakova) and [@mglikenf](https://github.com/mglikenf).

## Description
Minishell is a 42 School team project aimed at creating a basic shell program in C, designed to enhance understanding of system-level programming and teamwork. It implements redirections and pipes, as well as environment variable expansions and the `cd`, `echo`, `env`, `exit`, `export`, `pwd` and `unset` builtin commands.

## Features
Minishell is a miniature shell program inspired by Bash, designed to replicate its key functionalities:
* Prompt display
* Command history (up and down arrows)
* System executables available from the environment (`ls`, `cat`, `grep`, etc.)
* Local executables (`./minishell`)
* Builtin commands :
  * `echo` (and option `-n`)
  * `cd` (with only a relative or absolute path)
  * `pwd` (no options)
  * `export` (no options)
  * `unset` (no options)
  * `env` (no options or arguments)
  * `exit` (with exit number but no other options) 
* Pipes `|` which redirect output from one command to input for the next
* Redirections:
  * `>` redirects output
  * `>>` redirects output in append mode
  * `<` redirects input
  * `<< DELIMITER` displays a new prompt, reads user input until reaching `DELIMITER`, redirects user input to command input (does not update history)
* Environment variables (i.e. `$USER` or `$VAR`) that expand to their values.
  * `$?` expands to the exit status of the most recently executed foreground pipeline.
* User keyboard signals:
  * `ctrl-c` displays a new prompt line.
  * `ctrl-d` exits minishell
  * `ctrl-\` does nothing

However, Minishell does not support `\`, `;`, `&&`, `||`, or wildcards, as these features would require significantly more complex parsing logic and are outside the scope of this project.

## Suppression File
Using a suppression file helps Valgrind focus on actual memory leaks in your code by ignoring known issues in external libraries like `readline`. To run Minishell with Valgrind and suppress readline memory leaks, you can use a suppression file. Example command:

```bash
valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell
```

## Testing
We thoroughly tested the project using the **[minishell_tester](https://github.com/LucasKuhn/minishell_tester)**. Minishell successfully passes all tests.

*Subject version: 7.1.*
