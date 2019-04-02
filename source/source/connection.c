#include<stdio.h>
#include<math.h>
#include"connection.h"
extern int Now_Connection;
extern int Max_Connection;
char endMark='\0';
int connection_init(SOCKET* slisten)
{

	//��ʼ��WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	struct sockaddr_in sin;
	//SOCKET slisten;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		printf("��ʼ��WSA Error!");
		return -1;
	}
 
	//�����׽���
	*slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return -1;
	}
 
	//��IP�Ͷ˿�
	
	sin.sin_family = AF_INET;
	sin.sin_port = htons(LiSTEN_POINT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(*slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
		return -1;
	}
 
	//��ʼ����
	if (listen(*slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return -1;
	}
	return 0;
}
void start_run(SOCKET slisten)
{
	int nAddrlen;
	struct sockaddr_in remoteAddr;
	pthread_t tid;
	SocketContent* socketcontent;
	SOCKET sClient;
	//ѭ����������
	nAddrlen = sizeof(remoteAddr);
	printf("�ȴ�����...\n");
	while(true)
	{
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);//�ȴ����ӣ�������
		socketcontent=(SocketContent*)malloc_safe(sizeof(SocketContent));
		if(socketcontent==NULL)
		{
			printf("Memory allocation failed!\n");
			continue;
		}
		else if (sClient == INVALID_SOCKET)
		{
			printf("accept error !\n");
			free_safe(socketcontent);
			continue;
		}
		else if(Now_Connection>Max_Connection)
		{
			if(sendmessage(sClient,code_406)<0)
			{
				printf("send connection has Max error!\n");
			}
			closesocket(sClient);
			free_safe(socketcontent);
			continue;
		}
		Now_Connection++;
		socketcontent->sClient=sClient;
		socketcontent->nAddrlen=nAddrlen;
		socketcontent->remoteAddr=remoteAddr;
		pthread_create(&tid,NULL,(void*)recevice,socketcontent);
		printf("���ܵ�һ�����ӣ�%s port:%d \r\n", inet_ntoa(socketcontent->remoteAddr.sin_addr),sClient);
	}
	closesocket(slisten);
	WSACleanup();
	return; 
}
void recevice(SocketContent* sc)
{
	int ret,point=0;
	RevData* temp,*revData;
	Code code;
	revData=(RevData*)malloc_safe(sizeof(RevData));
	if(revData==NULL)
	{
		printf("memory malloc error!\n");
		closesocket(sc->sClient);
		free_safe(sc);
		Now_Connection--;
		return;
	}
	revData->next=NULL;
	temp=revData;
	//��������
	if(sendmessage(sc->sClient,code_201)<0)
	{
		printf("send error!\n");
	}
	while (true)
	{
		//��������
		ret = recv(sc->sClient, &temp->data[point], revData_len-point, 0);//���recv��copyʱ������ô������SOCKET_ERROR��
		if(ret==revData_len)
		{
			if(endMark==temp->data[revData_len-1])
			{
				//���ݽ������
				code=command_exec(revData);
				//��������
				if(sendmessage(sc->sClient,code)<0)
				{
					printf("send error!");
				}
				revData=(RevData*)malloc_safe(sizeof(RevData));
				if(revData==NULL)
				{
					printf("memory malloc error!\n");
					closesocket(sc->sClient);
					free_safe(sc);
					Now_Connection--;
					return;
				}
				revData->next=NULL;
				temp=revData;
			}
			else
			{
				temp->next=(RevData*)malloc_safe(sizeof(RevData));
				if(temp->next==NULL)
				{
					printf("memory malloc error!\n");
					closesocket(sc->sClient);
					free_safe(sc);
					Now_Connection--;
					return;
				}
				temp=temp->next;
				temp->next=NULL;
			}	
		}
		else if(ret>0)
		{
			if(endMark==temp->data[ret-1])
			{
				//temp->data[ret] = 0x00;
				//printfData(revData);
				//���ݽ������
				code=command_exec(revData);
				//��������
				if(sendmessage(sc->sClient,code)<0)
				{
					printf("send error!");
				}
				//printfData(revData);
				//��������
				//send(sc->sClient, sendData, strlen(sendData), 0);
				point=0;
				revData=(RevData*)malloc_safe(sizeof(RevData));
				if(revData==NULL)
				{
					printf("memory malloc error!");
					closesocket(sc->sClient);
					free_safe(sc);
					Now_Connection--;
					return;
				}
				revData->next=NULL;
				temp=revData;
			}
			else
			{
				point+=ret;
				if(point==revData_len)
				{
					temp->next=(RevData*)malloc_safe(sizeof(RevData));
					temp=temp->next;
					temp->next=NULL;
				}
			}
		}
		else
		{
			printf("�Ͽ����ӣ�%s \r\n", inet_ntoa(sc->remoteAddr.sin_addr));
			Now_Connection--;
			return;
		}
	}
	closesocket(sc->sClient);
	free_safe(sc);
}
void freeMemory(RevData* data)
{
	RevData* temp;
	while(data!=NULL)
	{
		temp=data->next;
		free_safe(data);
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
					printf("\n");
					return;
				}
				printf("%c",data->data[i]);
			}
			data=data->next;
		}
}

int send_code(SOCKET sClient,Code code)
{
	int len;
	char codestr[100];
	len=sprintf(codestr,code_template,code.code,code.key,NULL_CHAR,code.msg);
	if(len<0)
	{
		return -1;
	}
	else
	{
		send(sClient,codestr,len,0);
	}
	return 0;
}
int send_data(SOCKET sClent,Code code)
{
	int datalen=strlen(code.data),senddatalen,i,count;
	char* senddata;
	if(datalen<0)
	{
		return -1;
	}
	senddata=(char*)malloc_safe((100+datalen+strlen(code.msg))*sizeof(char));
	if(senddata==NULL)
	{
		return -1;
	}
	senddatalen=sprintf(senddata,code_template,code.code,code.key,code.data,code.msg);
	if(senddatalen<0)
	{
		return -1;
	}
	count=(int)ceil(1.0*senddatalen/SEND_MAX);
	for(i=0;i<count-1;i++)
	{
		send(sClent,senddata,SEND_MAX,i*SEND_MAX);
	}
	send(sClent,senddata,senddatalen-i*SEND_MAX,i*SEND_MAX);
	free_safe(senddata);
	return senddatalen;
	
}
int sendmessage(SOCKET sClient,Code code)
{
	if(code.data==NULL)
		return send_code(sClient,code);
	else
		return send_data(sClient,code);
}