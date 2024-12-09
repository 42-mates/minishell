#!/bin/bash

MINISHELL="./minishell"

GREEN="\033[0;32m"
RED="\033[0;31m"
BLUE="\033[0;34m"
PURPLE="\033[0;35m"
YELLOW="\033[0;93m"
RESET="\033[0m"

run_test() {
    command="$1"
    expected_status="$2"
    description="$3"

    echo -e "${YELLOW}$command${RESET}"

    # run command in minishell
    echo -e "$command\nexit\n" | $MINISHELL > minishell_output.txt 2>&1
    minishell_status=$?

    # run command in bash
    echo -e "$command\nexit\n" | bash > bash_output.txt 2>&1
    bash_status=$?

    # put under comment to check output
    # if [ "$minishell_status" -eq "$bash_status" ]; then
    #     status_result="${GREEN}PASS${RESET}"
    # else
    #     status_result="${RED}FAIL${RESET}"
    # fi

    # echo -e "$status_result: (minishell: $minishell_status, expected: $bash_status)"

    # put under comment to check only exit statuses
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

run_test "/bin/echo Hello, World!"
run_test "echo Hello, World!"

run_test "/bin/ls"
run_test "ps"
run_test "/bin/cat nonexistent_file"
run_test "/bin/sleep 1"

echo -e "\n===== Pipes =====\n"

run_test "/bin/echo 'Hello, Pipe!' | /usr/bin/grep Pipe"
run_test "/bin/ls | /usr/bin/wc -l"
run_test "/bin/echo 'Test' | nonexistent_command"
run_test "nonexistent_command | echo 'Test'"

echo -e "\n===== Builtins =====\n"

### echo
run_test "echo Hello, Builtin!"
run_test "echo -n 'No Newline'"
run_test "echo '-n' 'Test'"
run_test "echo -n -n -n HELLO"
run_test "echo -n HELLO -n -n"
run_test "echo HELLO -n -n"

### cd
mkdir test_dir
run_test "cd test_dir"
run_test "pwd"
run_test "cd nonexistent_dir"
run_test "cd restricted_dir"

### pwd
run_test "pwd"

### export и env
run_test "export TEST_VAR='Hello, Env!'"
run_test "env | grep TEST_VAR"
run_test "echo \$TEST_VAR"
run_test "export PWD=/temp"
run_test "export EMPTY_VAR"
run_test "export 1INVALID=foo"
run_test "export INVALID-NAME=foo"
run_test "export =foo"

### unset
run_test "unset TEST_VAR"
run_test "env | grep TEST_VAR"

run_test "chmod 777 nonexistent_file"

echo -e "\n===== Redirects =====\n"

### Output redirect >
run_test "/bin/echo 'Redirect Test' > output.txt"
run_test "cat output.txt"

### Input redirect <
run_test "echo 'Input Test' > input.txt"
run_test "/bin/cat < input.txt"

### Append redirect >>
run_test "/bin/echo 'Append Test' >> output.txt"
run_test "cat output.txt"

### Heredoc <<
run_test "cat << EOF
Heredoc Test
EOF" 0

echo -e "\n===== Quotes =====\n"

run_test "echo 'Single quotes test \$TEST_VAR'"
run_test "echo \"Double quotes test \$TEST_VAR\""
run_test "echo Backslash \\ test"
run_test "echo Unclosed quote test '"

echo -e "\n===== Env vars and \$? =====\n"

run_test "echo \$HOME"
run_test "echo \"\$HOME \$USER\""
run_test "echo '\$HOME \$USER'"
run_test "echo "\$HOME"'\$USER'"
run_test "echo '"\$HOME"'"
run_test "echo \"'\$HOME'\""
run_test "echo \$USER is logged in"
run_test "echo \$HOME'\$USER'"
run_test "echo \"\$HOME\"'is home'"
run_test "echo '\$USER' is user"
run_test "echo \"Combined \$USER\"' and plain'"
run_test "echo "\$HOME\$USER""
run_test "echo '\$HOME\$USER'"
run_test "echo \"User: \$USER and Home: \$HOME\""
run_test "echo \$."
run_test "echo \$="

run_test "export USER='Olga Prosviriakova' | export | grep USER"
run_test "echo \$?"
run_test "echo \$?HELLO"


# cleanup
rm -f restricted_file
chmod 700 restricted_dir
rmdir restricted_dir
rm -f output.txt input.txt bash_output.txt minishell_output.txt
rm -rf test_dir
