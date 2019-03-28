#include<time.h>
//include"../include/socket.h"
#include"code.h"
#include"dataTree.h"
Code insert(RevData* data);

const long converint[]={
(long)1,
(long)10,
(long)100,
(long)1000,
(long)10000,
(long)100000,
(long)1000000,
(long)10000000,
(long)100000000,
(long)1000000000,
(long)10000000000,
(long)100000000000,
(long)1000000000000,
(long)10000000000000,
(long)100000000000000,
(long)1000000000000000,
(long)10000000000000000,
(long)100000000000000000,
(long)1000000000000000000,
(long)10000000000000000000
};

time_t gettime(RevData* data)
{
	time_t time=0,mult=10;
	int i=0;
	char temp;
	char* point=data->data;
	while(*point!=' ')
	{
		point++;
	}
	point++;
	while(*point!=' ')
	{
		if(*point<'0'||*point>'9'||i>19)
		{
			return -1;
		}
		temp=*point<<3;
		temp=temp>>3;
		time+=temp*converint[i];
		point++;
		i++;
	}
	return time;
}
Code insert(RevData* data)
{
	Code code;	
	time_t extime=gettime(data),nowtime;
	if(time<0)
	{
		return code_402;
	}
	code.code=200;
	code.msg="Ìí¼Ó³É¹¦";
	nowtime=time(&nowtime);
	extime+=nowtime;
	insert_by_node(data_tree,create_node(count_node,NULL,NULL,data,extime));
	code.key=count_node++;
	return code;
}
