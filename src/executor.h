#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "command.h"

int execute_all(Command *commands, int num_commands, int background);

#endif