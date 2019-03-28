#include <string.h>
#include"command.h"

static char* command_all[4]={"INSERT","DELETE","UPDATE","GET"};
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
	char *p1,*p2;
	int i,temp=-1,temp1,count=0;
	for(i=0;i<4;i++)
	{
		p1=command_all[i];
		p2=command;
		if(strlen(p2)>strlen(p1))
			continue;
		temp1=count++;
		while(*p2!=0x00)
		{
			if(*p1==*p2)
			{
				p1++;
				p2++;
				continue;
			}
			else
			{
				count--;
				break;
			}
		}
		if(count>temp1)
		{
			temp=i;
		}
	}
	return count>1?-1:temp;
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
			return insert(data);
		}
	case 1:
		{
			return insert(data);
		}
	case 2:
		{
			return insert(data);
		}
	case 3:
		{
			return insert(data);
		}
	default:
		{
			return insert(data);
		}
	}
}
 