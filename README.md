# PipeDream

A minimal Unix-like command shell built from scratch in C.

## Project Description

PipeDream is a small command-line shell developed in C to understand how a shell works internally and how Linux processes and system calls are used to execute commands.

## Problem Statement

When users run commands in a Linux terminal, the shell handles tasks such as reading input, interpreting commands, and starting programs.

The goal of PipeDream is to build a simplified shell from scratch and understand the fundamental concepts involved in command execution.

## Goals

* Understand how a command-line shell works internally.
* Learn how user input is parsed into commands and arguments.
* Understand Linux processes and process creation.
* Learn how system calls are used to execute external commands.
* Practice C programming through a real system-level project.
* Build a small, modular, and maintainable shell.

## Planned Features

* Interactive command prompt
* Read and parse user commands
* Built-in commands such as `cd`, `pwd`, `echo`, and `exit`
* Execute external Linux commands
* Basic error handling
* Output redirection

Additional features may be added depending on project progress and available time.

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

* Linux or WSL
* GCC
* Make

## Build

```bash
make
```

## Run

```bash
./pipedream
```

## Clean

```bash
make clean
```

## Project Status

Currently under development.

## License

This project is licensed under the MIT License.

