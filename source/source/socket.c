#include<stdio.h>
#include"socket.h"

int main(int argc, char* argv[])
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	SOCKET slisten;
	struct sockaddr_in sin;
	SOCKET sClient;
	struct sockaddr_in remoteAddr;
	int nAddrlen;
	pthread_t tid;
	SocketContent* socketcontent;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
 
	//创建套接字
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}
 
	//绑定IP和端口
	
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	//sin.sin_addr.S_un.S_addr = INADDR_ANY;
	sin.sin_addr.S_un.S_un_b.s_b1=192;
	sin.sin_addr.S_un.S_un_b.s_b2=168;
	sin.sin_addr.S_un.S_un_b.s_b3=1;
	sin.sin_addr.S_un.S_un_b.s_b4=101;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
 
	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}
 
	//循环接收数据
	nAddrlen = sizeof(remoteAddr);
	printf("等待连接...\n");
	while(true)
	{
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);//等待连接（会阻塞）
		socketcontent=(SocketContent*)malloc(sizeof(SocketContent));
		socketcontent->sClient=sClient;
		socketcontent->nAddrlen=nAddrlen;
		socketcontent->remoteAddr=remoteAddr;
		pthread_create(&tid,NULL,(void*)recevice,socketcontent);
		printf("接受到一个连接：%s \r\n", inet_ntoa(socketcontent->remoteAddr.sin_addr));
	}
	closesocket(slisten);
	WSACleanup();
	return 0; 
}
void recevice(SocketContent* sc)
{
	int ret,point=0;
	char endMark=';';
	RevData* temp,*revData;
	char * sendData = "{\"Code\":\"200\";\"Msg\":\"Connection Successful!\"}";
	Code code;
	revData=(RevData*)malloc(sizeof(RevData));
	revData->next=NULL;
	temp=revData;
	if (sc->sClient == INVALID_SOCKET)
	{
		printf("accept error !");
		return;
	}
	//发送数据
	send(sc->sClient, sendData, strlen(sendData), 0);
	while (true)
	{
		//接收数据
		ret = recv(sc->sClient, &temp->data[point], revData_len-point, 0);//如果recv在copy时出错，那么它返回SOCKET_ERROR；
		//printf(ret);
		if(ret==revData_len)
		{
			if(endMark==temp->data[revData_len-1])
			{
				temp->next=(RevData*)malloc(sizeof(RevData));
				temp=temp->next;
				temp->next=NULL;
				temp->data[0]=0x00;
				printfData(revData);
				//发送数据
				send(sc->sClient, sendData, strlen(sendData), 0);
			}
			else
			{
				temp->next=(RevData*)malloc(sizeof(RevData));
				temp=temp->next;
				temp->next=NULL;
			}	
		}
		else if(ret>0)
		{
			if(endMark==temp->data[ret-1])
			{
				temp->data[ret] = 0x00;
				//数据接收完成
				code=command_exec(revData);
				//printfData(revData);
				//发送数据
				//send(sc->sClient, sendData, strlen(sendData), 0);
				point=0;
			}
			else
			{
				point+=ret;
				if(point==revData_len)
				{
					temp->next=(RevData*)malloc(sizeof(RevData));
					temp=temp->next;
					temp->next=NULL;
				}
			}
		}
		else
		{
			printf("断开连接：%s \r\n", inet_ntoa(sc->remoteAddr.sin_addr));
			return;
		}
	}
	closesocket(sc->sClient);
	free(sc);
}
void freeMemory(RevData* data)
{
	RevData* temp;
	while(data!=NULL)
	{
		temp=data->next;
		free(data);
		data=temp;
	}
}
void printfData(RevData* data)
{
	int i;
	while(data!=NULL)
		{
			for(i=0;i<revData_len;i++)
			{
				if(data->data[i]==0x00)
				{
					return;
				}
				printf("%c",data->data[i]);
			}
			data=data->next;
		}
}
