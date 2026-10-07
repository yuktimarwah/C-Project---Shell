# PipeDream

A lightweight cross-platform command shell built from scratch in C.

## Project Description

PipeDream is a small command-line shell developed in C to understand how a shell works internally and how operating-system-specific mechanisms are used to execute commands.

The project supports both Linux and Windows by providing a common set of commands and mapping them to the appropriate operating-system commands internally.

## Problem Statement

When users run commands in a terminal, the shell handles tasks such as reading input, interpreting commands, starting programs, and connecting command input and output.

The goal of PipeDream is to build a simplified shell from scratch while understanding fundamental concepts such as process creation, command execution, input/output redirection, and inter-process communication.

## Goals

* Understand how a command-line shell works internally.
* Learn how user input is parsed into commands and arguments.
* Understand process creation and command execution.
* Learn how operating-system-specific mechanisms are used to execute commands.
* Understand input/output redirection and command pipelines.
* Practice C programming through a real system-level project.
* Build a small, modular, and maintainable shell.

## Features

* Interactive command prompt
* Automatic operating system detection
* Common commands with OS-specific command mapping
* Built-in commands such as `cd`, `pwd`, `echo`, and `exit`
* External command execution
* Input redirection using `<`
* Output redirection using `>`
* Command pipelines using `|`
* Basic error handling

### Standard Commands

PipeDream provides common commands that work across supported operating systems:

| PipeDream Command | Linux   | Windows |
| ----------------- | ------- | ------- |
| `list`            | `ls`    | `dir`   |
| `show`            | `cat`   | `type`  |
| `clear`           | `clear` | `cls`   |

The shell detects the operating system and maps these commands to the appropriate native command.

### Cross-Platform Execution

PipeDream uses operating-system-specific execution mechanisms:

* **Linux:** Uses POSIX process and file-descriptor mechanisms such as `fork()`, `execvp()`, `pipe()`, and `dup2()`.
* **Windows:** Uses Windows process and handle APIs such as `CreateProcessA()` and native command execution through `cmd.exe`.

For pipelines, Linux uses POSIX pipes, while Windows delegates pipeline handling to the native Windows command processor.

## Project Structure

```text
PipeDream/
├── src/
│   └── main.c
├── include/
├── .gitignore
├── LICENSE
├── Makefile
└── README.md
```

## Prerequisites

### Linux / WSL

* GCC
* Make

### Windows

* MinGW-w64 / GCC

## Build

### Linux / WSL

```bash
make
```

### Windows

```bash
gcc -Wall -Wextra -Werror -std=c11 src/main.c -o PipeDream.exe
```

## Run

### Linux / WSL

```bash
./PipeDream
```

### Windows

```cmd
PipeDream.exe
```

## Clean

### Linux / WSL

```bash
make clean
```

## Project Status

Completed as a cross-platform C shell project supporting Linux and Windows.

## License

This project is licensed under the MIT License.
