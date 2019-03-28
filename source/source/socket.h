#include <winsock2.h>
#include <windows.h>
#include <pthread.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "pthreadVC2.lib") 
#include "command.h"

#ifndef _SOCKET_H_
#define _SOCKET_H_
#define true 1
#define false 0


typedef struct
{
	struct sockaddr_in remoteAddr;
	int nAddrlen;
	SOCKET sClient;
} SocketContent;

void recevice(SocketContent* sc);
void freeMemory(RevData* data);
void printfData(RevData* data);
#endif