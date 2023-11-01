#minishell

Minishell is 42 project that involves the development of our own custom bash version, which is designed to interpret user prompts, execute programs with the specified arguments, and include some built-in functions.

## Overview

This project aims to understand and reproduce the operation of a simplistic bash. It handles pipes, redirects, wildcards and environment variables like bash. You can run executables from an absolute, relative or environment ``PATH`` with arguments or options. The string delimiters (`'`, `"`) work the same as bash, except for multiline commands.

## Installation

Clone the repository:

```shell
# Clone this repository
git clone --recursive https://github.com/bbordere/minishell42.git

# Go into the repository
cd minishell42
```

## Compile and Run
You simply have to run: 
```shell
make
```

To run the program: 
```shell
./minishell
```

## Features
- `|`: Used to pass the result of one shell command to another shell command.
- `<`: Redirect input
- `<<`: Read the input until a line containing the delimiter is readed
- `>`: Redirect output
- `>>`: Redirect output in append mode
- `$VAR`: Environment variable including the return code ``$?``
- `CTRL-C`: Interrupt current process
- `CTRL-D`: Throw an ``EOF``

## Built-ins implemented
- `echo`: Print a line in the output
- `cd`: Change current directory
- `pwd`: Print the current directory path
- `export`: Add a variable to the environment 
- `unset`: Remove a variable from the environment 
- `env`: Print the environment
- `exit`: Exit the shell