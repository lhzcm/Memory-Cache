#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#define PORT 8888
#define true 1
#define false 0

int main()
{
	WORD sockVersion =MAKEWORD(2,2);
	WSADATA wsaData;
	SOCKET slisten;
	struct sockaddr_in sin;
	SOCKET sclient;
	struct sockaddr_in remoteAddr;
	int nAddrlen,ret;
	char * sendData="200";
	if(WSAStartup(sockVersion,&wsaData)!=0)
	{
		printf("create connection faile!\n");
		return 0;
	}
	slisten = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(slisten==INVALID_SOCKET)
	{
		printf("create socket faile!\n");
		return 0;
	}
	sin.sin_family=AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.S_un.S_addr=INADDR_ANY;
	if(bind(slisten,(LPSOCKADDR)&sin,sizeof(sin))==SOCKET_ERROR)
	{
		printf("bind ip address and port faile\n");
		return 0;
	}
	if(listen(slisten,5)==SOCKET_ERROR)
	{
		printf("Listen falie!\n");
		return 0;
	}
	nAddrlen=sizeof(remoteAddr);
	printf("waitting connection!\n");
	sclient=accept(slisten,(SOCKADDR *)&remoteAddr,&nAddrlen);
	while(true)
	{
		char revData[10];
		if(sclient==INVALID_SOCKET)
		{
			printf("Accept faile!\n");
			continue;
		}
		printf("accept a new connection:%s\r\n",inet_ntoa(remoteAddr.sin_addr));
		ret = recv(sclient,revData,10,0);
		if(ret>0)
		{
			revData[ret]='\0';
			printf(revData);
		}
		send(sclient,sendData,strlen(sendData),0);
	}
	closesocket(sclient);
	closesocket(slisten);
	WSACleanup();
	return 0;
}
