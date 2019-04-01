#include <winsock2.h>
#include <windows.h>
#include <pthread.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "pthreadVC2.lib") 
#define true 1
#define false 0
#define revData_len 10

typedef struct
{
	struct sockaddr_in remoteAddr;
	int nAddrlen;
	SOCKET sClient;
} SocketContent;
typedef struct RevData
{
	char data[revData_len];
	struct RevData* next;
}RevData;

void recevice(SocketContent* sc);
void freeMemory(RevData* data);
void printfData(RevData* data);