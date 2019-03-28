//#include <stdio.h>
#include"command.h"


void command_read(RevData* data,char* command)
{
	int i=0;
	while(i<10&&data!=NULL)
	{
		if(data->data[i]==' ')
		{
			command[i]=0x00;
			break;
		}
		command[i]=data->data[i]&0xDF;
		i++;
	}
}
int command_find(char* command)
{
	char* p1;
	char* p2;
	int i;
	for(i=0;i<4;i++)
	{
		p1=command_all[i];
		p2=command;
		if(*p2==0x00)
			return -1;
		while(*(p1-1)!=0x00)
		{
			if(*p2==0x00)
				return i;
			if(*p1==*p2)
			{
				p1++;
				p2++;
				continue;
			}
			else
				break;
		}
	}
	return -1;
}
Code command_exec(RevData* data)
{
	int i;
	char command[10];
	command_read(data,command);
	i=command_find(command);
	switch(i)
	{
	case 0:
		{
			printf("insert");
			return insert(data);
		}
	case 1:
		{
			printf("delete");
			return code_500;
		}
	case 2:
		{
			printf("update");
			return code_500;
		}
	case 3:
		{
			printf("get");
			return code_500;
		}
	default:
		{
			return code_500;
		}
	}
}
 