//#include "../include/socket.h"
#include "code.h"
#include "core.h"
char* commands[4]={"INSERT","DELETE","UPDATE","GET"};
void command_read(RevData* data,char* command);
int command_find(char* command);
Code command_exec(RevData* data);