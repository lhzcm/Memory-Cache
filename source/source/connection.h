#include <winsock2.h>
#include <windows.h>
#include <pthread.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "pthreadVC2.lib") 
#include "command.h"
#include "code.h"

#ifndef _SOCKET_H_
#define _SOCKET_H_
#define true 1
#define false 0
#define LiSTEN_POINT 2333
#define SEND_MAX 1024


typedef struct
{
	struct sockaddr_in remoteAddr;
	int nAddrlen;
	SOCKET sClient;
} SocketContent;

void recevice(SocketContent* sc);
void freeMemory(RevData* data);
void printfData(RevData* data);
int sendmessage(SOCKET sClient,Code code);
int connection_init();
void start_run(SOCKET slisten);

#endif