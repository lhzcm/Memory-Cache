#include<stdio.h>
#include"socket.h"

int main(int argc, char* argv[])
{
	//��ʼ��WSA
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
 
	//�����׽���
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}
 
	//��IP�Ͷ˿�
	
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
 
	//��ʼ����
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}
 
	//ѭ����������
	nAddrlen = sizeof(remoteAddr);
	printf("�ȴ�����...\n");
	while(true)
	{
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);//�ȴ����ӣ���������
		socketcontent=(SocketContent*)malloc(sizeof(SocketContent));
		socketcontent->sClient=sClient;
		socketcontent->nAddrlen=nAddrlen;
		socketcontent->remoteAddr=remoteAddr;
		pthread_create(&tid,NULL,(void*)recevice,socketcontent);
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(socketcontent->remoteAddr.sin_addr));
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
	//��������
	send(sc->sClient, sendData, strlen(sendData), 0);
	while (true)
	{
		//��������
		ret = recv(sc->sClient, &temp->data[point], revData_len-point, 0);//���recv��copyʱ������ô������SOCKET_ERROR��
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
				//��������
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
				//���ݽ������
				code=command_exec(revData);
				//printfData(revData);
				//��������
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
			printf("�Ͽ����ӣ�%s \r\n", inet_ntoa(sc->remoteAddr.sin_addr));
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
