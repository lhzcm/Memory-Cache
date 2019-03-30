#include <string.h>
#include"command.h"

char* command_all[]={"INSERT","DELETE","UPDATETIME","UPDATEDATA","UPDATEALL","GET"};

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
	for(i=0;i<6;i++)
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
			return insert_data(data);
			break;
		}
	case 1:
		{
			return delete_data(data);
			break;
		}
	case 2:
		{
			return updata_time(data);
			break;
		}
	case 3:
		{
			return updata_data(data);
			break;
		}
	case 4:
		{
			return updata_all(data);
			break;
		}
	case 5:
		{
			return get_data(data);
			break;
		}
	default:
		{
			return code_201;
			break;
		}
	}
}
 