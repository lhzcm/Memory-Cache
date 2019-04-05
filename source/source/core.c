#include <time.h>
#include<stdio.h>
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
static pthread_mutex_t datatree_mutex=PTHREAD_MUTEX_INITIALIZER;
//extern pthread_mutex_t malloc_free_mutex; //malloc mutex
//extern pthread_mutex_t free_mutex; //free mutex
extern time_t session_time;


Code code_init(unsigned short codeno,unsigned int key,char* data,char* msg)
{
	Code code;
	code.code=codeno;
	code.key=key;
	code.data=data;
	code.msg=msg;
	return code;
}

time_t get_time(RevData* data,int position)
{
	time_t time=0,mult=10;
	int i;
	char* point=data->data;
	for(i=0;i<position;i++)
	{
		while(*(point++)!=' '){}	
	}
	while(*(point+1)!=0x00&&*(point+1)!=' ')
	{
		point++;
	}
	i=0;
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
unsigned int get_key(RevData* data)
{
	unsigned int key=0;
	int i=0;
	char* point=data->data;
	while(*(point++)!=' '){}
	while(*(point+1)!='\0'&&*(point+1)!=' ')
	{
		point++;
	}
	while(*point!=' ')
	{
		if(*point<'0'||*point>'9'||i>9)
		{
			return 0;
		}

		key+=numbers[*point-0x30]*converint[i];
		point--;
		i++;
	}
	return key;
}
int get_RevData_len(RevData* data,int position)
{
	int count=0,i;
	char* point=data->data;
	for(i=0;i<position;i++)
	{
		while(*(point++)!=' '){}
	}
	while(point<(data->data+revData_len))
	{
		if(*point==0x00)
			return count;
		count++;
		point++;
	}
	data=data->next;
	while(data!=NULL)
	{
		if(data->next!=NULL)
		{
			count+=revData_len;
			data=data->next;
		}
		else
		{
			point=data->data;
			while(*point==0x00)
			{
				count++;
				point++;
			}
			return count;
		}
	}
	return -1;
}
int RevDataToChar(RevData* data,char** chardata,int position)
{
	int i,length;
	char* point=data->data;
	for(i=0;i<position;i++)
	{
		while(*(point++)!=' '){}
	}
	length=get_RevData_len(data,position);
	if(length<=0)
	{
		return -1;
	}
	*chardata =(char*)malloc_safe((length+1)*sizeof(char));
	if(*chardata==NULL)
	{
		printf("memory malloc error!\n");
		return -1;
	}
	(*chardata)[length]=0x00;
	
	i=0;
	while(point<(data->data+revData_len))
	{
		if(i>=length)
		{
			return i;
		}
		(*chardata)[i]=*point;
		i++;
		point++;
	}
	data=data->next;
	while(data!=NULL)
	{
		point=data->data;
		while(i<length&&point<(data->data+revData_len))
		{
			(*chardata)[i]=*point;
			i++;
			point++;
		}
		data=data->next;
	}
	return i;
}
void free_RevData(RevData* data)
{
	RevData* temp;
	while(data!=NULL)
	{
		temp=data->next;
		free_safe(data);
		data=temp;
	}
}
int time_check(dataTree* node)
{
	time_t now=time(NULL);
	int errorcode;
	if(node->expire==0||node->expire>=now)
	{
		if(node->expire<session_time+now)
			node->expire=now+session_time;
		return 0;
	}
	else
	{
		pthread_mutex_lock(&datatree_mutex); //添加线程锁
		data_tree=delete_by_key(data_tree,node->key,&errorcode);
		pthread_mutex_unlock(&datatree_mutex);
		return -1;
	}
}

Code insert_data(RevData* data)
{
	char* chardata;
	dataTree* node;
	time_t extime=get_time(data,1),nowtime;
	if(extime<0)
	{
		return code_503;
	}
	if(RevDataToChar(data,&chardata,2)<0)
	{
		return code_504;
	}
	pthread_mutex_lock(&datatree_mutex);  //添加线程锁
	//for(i=0;i<threadnum;i++)
	//{
		//sem_wait(&sem);
	//}
	if(extime>0)
	{
		nowtime=time(NULL);
		extime+=nowtime;
	}
	node=create_node(count_node,NULL,NULL,chardata,extime);
	if(node==NULL)
	{
		//for(i=0;i<threadnum;i++)
		//{
			//sem_post(&sem);
		//}
		return code_501;
	}
	data_tree=insert_by_node(data_tree,node);
	count_node++;
	pthread_mutex_unlock(&datatree_mutex);
	//printf("%d\n",count_node);
	//for(i=0;i<threadnum;i++)
	//{
		//sem_post(&sem);
	//}
	free_RevData(data);
	
	return code_init(200,count_node,NULL,"insert successful!");//添加成功
}
Code get_data(RevData* commdata)
{
	unsigned int key;
	dataTree* node;
	key=get_key(commdata);
	if(key==0)
	{
		return code_502;
	}
	//sem_wait(&sem);
	node=get_by_key(data_tree,key);
	//sem_post(&sem);
	if(node==NULL||time_check(node)<0)
	{
		printf("%d=404\n",key);
		return code_404;
	}
	free_RevData(commdata);
	return code_init(200,key,node->data,"get data successful!");//成功查找到数据
}
Code delete_data(RevData* commdata)
{
	unsigned int key;
	int errorcode;
	key=get_key(commdata);
	if(key==0)
	{
		return code_502;
	}
	pthread_mutex_lock(&datatree_mutex); //添加线程锁
	data_tree=delete_by_key(data_tree,key,&errorcode);
	pthread_mutex_unlock(&datatree_mutex);
	if(errorcode<0)
	{
		return code_405;
	}
	free_RevData(commdata);
	return code_init(200,key,NULL,"delete data successful!");//成功删除数据
}
Code updata_time(RevData* commdata)
{
	unsigned int key=get_key(commdata);
	time_t extime=get_time(commdata,2),nowtime;
	dataTree* node;
	if(extime<0)
	{
		return code_503;
	}
	else if(key==0)
	{
		return code_502;
	}
	
	node=get_by_key(data_tree,key);
	if(node==NULL)
	{
		return code_404;
	}
	if(extime>0)
	{
		nowtime=time(&nowtime);
		extime+=nowtime;
	}
	node->expire=extime;
	free_RevData(commdata);
	return code_init(200,key,NULL,"update time successful");//成功更新时间
}
Code updata_data(RevData* commdata)
{
	unsigned int key=get_key(commdata);
	char* chardata;
	dataTree* node;
	if(key==0)
	{
		return code_502;
	}
	node=get_by_key(data_tree,key);
	if(node==NULL)
	{
		return code_404;
	}
	else if(RevDataToChar(commdata,&chardata,2)<0)
	{
		return code_504;
	}
	node->data=chardata;
	free_RevData(commdata);
	return code_init(200,key,NULL,"update data successful");//成功更新数据
}

Code updata_all(RevData* commdata)
{
	unsigned int key=get_key(commdata);
	time_t extime=get_time(commdata,2),nowtime;
	char* chardata;
	dataTree* node;
	if(extime<0)
	{
		return code_503;
	}
	else if(key==0)
	{
		return code_502;
	}
	node=get_by_key(data_tree,key);
	if(node==NULL)
	{
		return code_404;
	}
	else if(RevDataToChar(commdata,&chardata,3)<0)
	{
		return code_504;
	}
	if(extime>0)
	{
		nowtime=time(&nowtime);
		extime+=nowtime;
	}
	node->data=chardata;
	node->expire=extime;
	free_RevData(commdata);
	return code_init(200,key,NULL,"update data successful");//更新时间和数据
}
