# Minishell

A small shell written in C for the 42 school project. This project implements a simplified command interpreter with support for pipes, redirections, environment variables, heredocs, and builtins.

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Architecture](#architecture)
- [Build](#build)
- [Usage](#usage)
- [Supported Builtins](#supported-builtins)
- [Signal Handling](#signal-handling)
- [Source Organization](#source-organization)
- [License](#license)

## Description

`minishell` is a minimal command interpreter that reproduces part of the behavior of a Unix shell. The project focuses on parser construction, process management, input/output handling, and variable/redirection processing.

## Features

- External command execution
- Pipes (`|`)
- Input and output redirections (`<`, `>`, `>>`)
- Heredoc (`<<`)
- Variable expansion (`$VAR`)
- Wildcard expansion and word splitting
- Subshell command grouping with parentheses
- Logical operators (`&&`, `||`)
- Syntax and redirection error handling
- Standard builtins: `cd`, `echo`, `pwd`, `env`, `export`, `unset`, `exit`
- Line editing and history using `readline`

## Architecture

The project is organized into modules:

- `LEXER/`: tokenizes the command line
- `PARSER/`: parses and structures commands
- `EXPAND/`: handles variable expansion, wildcards, and word splitting
- `REDIRECT/`: manages redirections and pipes
- `EXEC/`: executes commands and pipelines
- `BUILTIN/`: implements internal shell commands
- `HEREDOC/`: supports heredoc behavior
- `SIGNALS/`: manages Unix signal handling
- `LIST/`: list structures for tokens, PIDs, and environment variables
- `UTILS/`: utility functions and error handling

## Build

### Requirements

- `gcc` or `cc`
- `make`
- `libreadline` installed on the system

### Command

```bash
make
```

The produced executable is named `minishell`.

### Clean

```bash
make clean
make fclean
```

## Usage

Start the shell:

```bash
./minishell
```

Example commands:

```bash
echo "Hello"
ls -la | grep src
cat < input.txt > output.txt
export PATH=/usr/bin
unset VARIABLE
cd /
pwd
```

## Supported Builtins

- `cd`
- `echo`
- `pwd`
- `env`
- `export`
- `unset`
- `exit`

## Signal Handling

The shell handles interruptions and signals to remain stable during interactive command execution. `Ctrl+C` and `Ctrl+\` are supported to preserve behavior similar to a standard shell.

## Source Organization

Source code is organized under `SRC/` with thematic subdirectories:

- `SRC/BUILD_EXEC/`
- `SRC/BUILTIN/`
- `SRC/CLEAN/`
- `SRC/EXEC/`
- `SRC/EXPAND/`
- `SRC/GNL/`
- `SRC/HEREDOC/`
- `SRC/LEXER/`
- `SRC/LIST/`
- `SRC/PARSER/`
- `SRC/REDIRECT/`
- `SRC/SIGNALS/`
- `SRC/UTILS/`

Header files are located in `INCLUDES/`.

## License

This project is provided without an explicit license. Use and modification are intended for educational purposes.
