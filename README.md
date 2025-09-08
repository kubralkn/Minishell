# Minishell

Minishell is a simplified shell project developed as part of the 42 curriculum.
The goal of this project is to implement a basic shell that mimics Bash behavior: handling user input, parsing, executing commands, and managing environment variables, while following strict coding standards.

Features

Prompt display for user input

Command execution with absolute/relative paths and PATH lookup

Built-in commands:

echo with -n option

cd with relative and absolute paths

pwd

export

unset

env

exit

Redirections: <, >, >>, << (heredoc)

Pipes | to connect commands

Environment variable expansion ($VAR)

Signal handling:

Ctrl-C interrupts

Ctrl-\ quits

Ctrl-D exits shell gracefully

Error handling for invalid commands, syntax errors, and memory leaks

Technical Requirements

Written in C under the 42 Norm

No memory leaks (checked with valgrind)

No forbidden functions

Organized structure (lexer, parser, executor, builtins, signals, utils, etc.)
