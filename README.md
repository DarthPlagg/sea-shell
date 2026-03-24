# 🐚 Sea Shell

A lightweight Unix shell written in C, focused on process management, IPC, and memory safety.

## Features

- Execute commands via `fork` + `execvp`
- Multi-stage pipelines (`|`) using `pipe` and `dup2`
- I/O redirection (`<`, `>`)
- Background execution (`&`)
- Built-ins: `cd`, `exit`, `clear`
- Dynamic input (no fixed limits)

## Technical Highlights

- Process lifecycle management with proper `wait` handling (no zombie processes)
- File descriptor control for pipelines and redirections (`dup2`)
- Multi-process pipelines with correct descriptor inheritance
- Dynamic memory management with full cleanup paths
- Zero memory leaks verified with Valgrind

## Architecture
src/
- main.c
- input.c
- parser.c
- executor.c
- command.h

## Build
```bash
git clone https://github.com/DarthPlagg/sea-shell.git
cd sea-shell
make
```
## Run
```bash
./seashell
```
# Example
```bash
ls -l | grep ".c" | wc -l
cat file.txt > out.txt
sleep 10 &
```
## Future Work
Signal handling (SIGINT)
Job control
Command history

---