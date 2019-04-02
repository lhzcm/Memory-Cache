#include<stdio.h>
#include"connection.h"
int Max_Connection;
int Now_Connection;
int config_init()
{
	Max_Connection=4;
	Now_Connection=0;
	return 0;
}
int main()
{
	SOCKET sever_sockt;
	config_init();
	if(connection_init(&sever_sockt)==-1)//初始化socket连接
	{
		printf("init connection faile!\n");
	}
	start_run(sever_sockt);

}