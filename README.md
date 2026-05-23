# Parallel Matrix Multiplication using IPC

A parallel matrix multiplication implemented in C using Linux Inter-Process Communication (IPC) mechanisms.

In the project we are using multiple child processes and anonymous pipes
for inter-process communication.

## Technologies

- C
- Linux System Calls
- fork()
- pipe()
- read/write()
- wait()

## Features

- Dynamic matrix dimensions
- Matrix compatibility validation
- Parallel computation using child processes
- IPC communication using pipes
- Parent-child synchronization
- Random matrix generation

## How It Works

1. The parent process creates two matrices with random values.
2. The program checks if the matrices can be multiplied.
3. For each element of the result matrix:
   - A child process is created using `fork()`
   - The parent sends the row and column indices through a pipe
   - The child calculates the result for that matrix element
   - The child sends the result back to the parent through another pipe
4. The parent stores the result in the final matrix.
5. After all calculations are completed, the final matrix is printed.


## Running the Project

This project is designed for Linux environments.

### Compile

Open a terminal in the project directory and run:

```bash
gcc matrix_multiplication_ipc.c -o matrix

./matrix
```

The program will ask for matrix dimensions through terminal input.
