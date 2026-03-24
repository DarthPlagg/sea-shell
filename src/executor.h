#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "command.h"

// Main execution engine: handles built-ins, pipes, and external programs
int execute_all(Command *commands, int num_commands, int background);

#endif