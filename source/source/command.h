#include "code.h"
#include "core.h"

#ifndef _COMMAND_H_
#define _COMMAND_H_
void command_read(RevData* data,char* command);
int command_find(char* command);
Code command_exec(RevData* data);
#endif