# 🐚 Sea Shell 

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

## 🧠 Technical Challenges & Learning Outcomes

Building a shell from scratch is a dive into the deep end of systems programming. Here are some of the key technical hurdles I overcame:

* **Process Orchestration & Synchronization:** Implementing pipes (`|`) required a deep understanding of `fork()` and `pipe()`. The challenge was ensuring that each process in the pipeline correctly inherits file descriptors and that the parent process waits only for the foreground tasks, preventing "zombie" processes.
* **File Descriptor Management:** Managing redirections (`<`, `>`) involved swapping standard streams using `dup2()`. Ensuring that pipes and files are closed in the correct order was crucial to prevent resource leaks and hanging processes.
* **Dynamic Memory Safety:** Since user input can be of any length, I implemented a dynamic buffer with `realloc()`. The challenge was ensuring that all memory—from the raw input string to the complex `Command` structures—is properly freed, even when a command fails or the user exits.
* **Recursive Signal Handling (Mental Model):** While not fully exposed in the current version, designing the architecture to eventually handle `SIGINT` (Ctrl+C) without killing the shell itself required careful planning of how processes are grouped.
* **Memory Safety & Leak Detection:** Every core component was rigorously tested using **Valgrind Memcheck**. By implementing a custom cleanup logic (`free_pipeline`), I ensured that even in complex edge cases—such as failed executions or deep command pipelines—the shell maintains a zero-leak profile, achieving the "All heap blocks were freed" status.

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


