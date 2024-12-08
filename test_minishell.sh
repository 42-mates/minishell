#!/bin/bash

MINISHELL="./minishell"

GREEN="\033[0;32m"
RED="\033[0;31m"
BLUE="\033[0;34m"
PURPLE="\033[0;35m"
RESET="\033[0m"

run_test() {
    command="$1"
    expected_status="$2"
    description="$3"

    echo "$command"

    # run command in minishell
    echo -e "$command\nexit\n" | $MINISHELL > minishell_output.txt 2>&1
    minishell_status=$?

    # run command in bash
    echo -e "$command\nexit\n" | bash > bash_output.txt 2>&1
    bash_status=$?

    if [ "$minishell_status" -eq "$expected_status" ]; then
        status_result="${GREEN}PASS${RESET}"
    else
        status_result="${RED}FAIL${RESET}"
    fi

    echo -e "$status_result: (minishell: $minishell_status, expected: $expected_status, bash: $bash_status)"

    echo -e "\n${BLUE}Minishell Output:${RESET}"
    cat minishell_output.txt
    echo -e "\n${PURPLE}Bash Output:${RESET}"
    cat bash_output.txt
    echo -e "${RESET}----------------------------------------\n"
}

touch restricted_file
chmod -x restricted_file

mkdir restricted_dir
chmod 000 restricted_dir

echo -e "\n===== Simple commands =====\n"

run_test "/bin/echo Hello, World!" 0
run_test "echo Hello, World!" 0

run_test "/bin/ls" 0
run_test "ps" 0
run_test "/bin/cat nonexistent_file" 1
run_test "/bin/sleep 1" 0

echo -e "\n===== Pipes =====\n"

run_test "/bin/echo 'Hello, Pipe!' | /usr/bin/grep Pipe" 0
run_test "/bin/ls | /usr/bin/wc -l" 0 
run_test "/bin/echo 'Test' | nonexistent_command" 127
run_test "nonexistent_command | echo 'Test'" 0

echo -e "\n===== Builtins =====\n"

### echo
run_test "echo Hello, Builtin!" 0
run_test "echo -n 'No Newline'" 0
run_test "echo '-n' 'Test'" 0
run_test "echo -n -n -n HELLO" 0
run_test "echo -n HELLO -n -n" 0
run_test "echo HELLO -n -n" 0

### cd
mkdir test_dir
run_test "cd test_dir" 0
run_test "pwd" 0 
run_test "cd nonexistent_dir" 1 
run_test "cd restricted_dir" 1

### pwd
run_test "pwd" 0

### export и env
run_test "export TEST_VAR='Hello, Env!'" 0
run_test "env | grep TEST_VAR" 0
run_test "echo \$TEST_VAR" 0
run_test "export PWD=/temp" 0
run_test "export EMPTY_VAR" 0
run_test "export 1INVALID=foo" 1
run_test "export INVALID-NAME=foo" 1
run_test "export =foo" 1

### unset
run_test "unset TEST_VAR" 0
run_test "env | grep TEST_VAR" 1

run_test "chmod 777 nonexistent_file" 1

echo -e "\n===== Redirects =====\n"

### Output redirect >
run_test "/bin/echo 'Redirect Test' > output.txt" 0
run_test "cat output.txt" 0

### Input redirect <
run_test "echo "Input Test" > input.txt"
run_test "/bin/cat < input.txt" 0

### Append redirect >>
run_test "/bin/echo 'Append Test' >> output.txt" 0
run_test "cat output.txt" 0

### Heredoc <<
# run_test "cat << EOF
# Heredoc Test
# EOF" 0

echo -e "\n===== Quotes =====\n"

run_test "echo 'Single quotes test \$TEST_VAR'" 0
run_test "echo \"Double quotes test \$TEST_VAR\"" 0
run_test "echo Backslash \\ test" 0
run_test "echo Unclosed quote test '" 2

echo -e "\n===== Env vars and \$? =====\n"

run_test "echo \$HOME" 0
run_test "echo "\$HOME \$USER"" 0
run_test "echo '\$HOME \$USER'" 0
run_test "echo "\$HOME"'\$USER'" 0
run_test "echo '"\$HOME"'" 0
run_test "echo "'\$HOME'"" 0
run_test "echo \$USER is logged in" 0
run_test "echo \$HOME'\$USER'" 0

run_test "export USER='Olga Prosviriakova' | export | grep USER" 0
run_test "echo \$?" 0


# cleanup
rm -f restricted_file
chmod 700 restricted_dir
rmdir restricted_dir
rm -f output.txt input.txt bash_output.txt minishell_output.txt
rm -rf test_dir
