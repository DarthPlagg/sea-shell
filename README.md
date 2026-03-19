# 🌊 Sea Shell (seash)

A lightweight, custom Unix shell written from scratch in C. 
This project was built to deeply understand operating system concepts such as process creation (`fork`, `execvp`, `waitpid`), dynamic memory management, and parsing user input.

## ✨ Features

* **Custom Prompt:** Features a dynamic, colored command-line prompt displaying the current working directory.
* **Process Execution:** Can execute standard Unix system commands (e.g., `ls`, `pwd`, `cat`).
* **Built-in Commands:**
  * `cd <directory>` - Change the current working directory.
  * `clear` - Clears the terminal screen using ANSI escape codes.
  * `exit` - Safely terminates the shell.
* **Dynamic Memory Allocation:** Handles user input and tokenization dynamically without hardcoded limits for the line length.
* **Modular Architecture:** Codebase is cleanly separated into logical modules for maintainability.

## 🛠️ Project Structure

* `src/main.c` - Entry point and main shell loop.
* `src/input.c` / `.h` - Handles reading raw user input dynamically.
* `src/parser.c` / `.h` - Tokenizes the input string into arguments.
* `src/executor.c` / `.h` - Manages built-in commands and forks new processes.
* `src/colors.h` - Contains ANSI color codes for terminal formatting.
* `Makefile` - Automates the compilation process.
* `.gitignore` - Ignores compiled binaries and object files.

## 🚀 Getting Started

### Prerequisites
You need a C compiler (like `gcc`) and `make` installed on your system.

### Installation & Compilation

1. **Clone the repository:** 
```Bash
   git clone [https://github.com/DarthPlagg/sea-shell.git](https://github.com/DarthPlagg/sea-shell.git)
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


