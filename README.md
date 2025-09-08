# Minishell

Minishell is a simplified shell project developed as part of the 42 curriculum.
The goal of this project is to create a basic shell implementation that mimics the behavior of Bash, handling user input, parsing, executing commands, and managing environment variables — all while following strict coding standards.

Features

Prompt Display – Shows a command prompt waiting for user input.

Command Execution – Supports execution of binaries from PATH as well as absolute and relative paths.

Built-in Commands:

echo with -n option

cd with relative and absolute paths

pwd (prints working directory)

export (set environment variables)

unset (remove environment variables)

env (display environment variables)

exit

Redirections:

< (input redirection)

> (output redirection, truncate)

>> (output redirection, append)

<< (heredoc)

Pipes (|) – Connects the output of one command to the input of another.

Environment Variables – Handles $VAR expansion.

Signal Handling:

Ctrl-C → interrupts current process

Ctrl-\ → quits current process

Ctrl-D → exits the shell gracefully

Error Handling – Manages invalid commands, syntax errors, and memory leaks.
