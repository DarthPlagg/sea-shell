# 🐚 Sea Shell 🌊

A lightweight, custom Unix shell written from scratch in C. 
This project was built to deeply understand operating system concepts such as process creation (`fork`, `execvp`), inter-process communication (`pipe`), file descriptors, and dynamic memory management.

## ✨ Features

* **Custom Prompt:** A dynamic, colored prompt displaying the current working directory in green and a bold cyan shell name.
* **Process Execution:** Executes standard Unix system commands (e.g., `ls`, `grep`, `cat`) using `fork` and `execvp`.
* **Advanced Pipelines:** Supports chaining multiple commands using the `|` operator (e.g., `cat file.txt | grep "search" | wc -l`).
* **I/O Redirections:** Supports standard input and output redirection using `<` and `>` operators.
* **Background Execution:** Run processes in the background by appending `&` to your command.
* **Built-in Commands:**
    * `cd <directory>` - Change the current working directory.
    * `clear` - Clears the terminal screen using ANSI escape codes.
    * `exit` - Safely terminates the shell.
* **Robust Memory Management:** Dynamic allocation for user input and command structures with zero hardcoded limits on line length or argument count.
* **Clean Architecture:** Modular codebase with clear separation between parsing and execution logic.

## 🛠️ Project Structure

* `src/main.c` - Orchestrates the main shell loop and handles the high-level lifecycle.
* `src/input.c / .h` - Reads raw user input character-by-character (handles `EOF` / `Ctrl+D` gracefully).
* `src/parser.c / .h` - Converts raw strings into a structured `Command` pipeline.
* `src/executor.c / .h` - Manages process forking, pipe creation, redirections, and built-ins.
* `src/command.h` - Defines the core `Command` structure used across the project.
* `src/colors.h` - Defines ANSI color constants for a modern terminal look.
* `Makefile` - Handles automated compilation and cleanup.

## 🚀 Getting Started

### Prerequisites
You need a C compiler (like `gcc`) and `make` installed on your system.

### Installation & Compilation

1. **Clone the repository:** 
```Bash
git clone https://github.com/DarthPlagg/sea-shell.git
```
2.**Navigate to the directory:**
```Bash
cd sea-shell
```
3.**Compile the project using Make:**
```Bash
make
```
4.**Run Sea Shell:**
```Bash
./seashell
```

## 🧹 Cleaning Up

To remove the compiled object files and the executable (to keep your workspace clean), run:

```bash
make clean
```


