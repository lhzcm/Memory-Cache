#include <time.h>
#include "core.h"
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
const int numbers[]={0,1,2,3,4,5,6,7,8,9};

time_t gettime(RevData* data)
{
	time_t time=0,mult=10;
	int i=0;
	char* point=data->data;
	while(*(point++)!=' '){}
	while(*(point+1)!=' ')
	{
		point++;
	}
	while(*point!=' ')
	{
		if(*point<'0'||*point>'9'||i>19)
		{
			return -1;
		}

		time+=numbers[*point-0x30]*converint[i];
		point--;
		i++;
	}
	return time;
}
Code insert(RevData* data)
{
	Code code;
	int error=0;
	time_t extime=gettime(data),nowtime;
	if(time<0)
	{
		return code_402;
	}
	code.code=200;
	code.msg="Ìí¼Ó³É¹¦";
	if(extime>0)
	{
		nowtime=time(&nowtime);
		extime+=nowtime;
	}
	data_tree=insert_by_node(data_tree,create_node(count_node,NULL,NULL,data,extime),&error);
	code.key=count_node++;
	return code;
}
