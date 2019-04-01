#include<stdio.h>
#include<pthread.h>
#include<Windows.h>
#pragma comment(lib, "pthreadVC2.lib")   //必须加上这句
 
void*Function_t(int* a) 
{
	 char* c="my name is good";
     pthread_t myid = pthread_self();
     while(1) 
     {
		 printf("a=%d\n",*a);
		 Sleep(4000);
         printf("线程ID=%d\n", myid);
         
     }
     return NULL;
}
//mullty params
int simple(int num,...)
{
	/*int i,result=0;
	va_list vl;
	va_start(vl,num);
	printf("num:%d,vl:%d\n",num,*vl);
	for(i=0;i<(num-1);i++)
	{
		result=va_arg(vl,int);
		printf("in for result:%d, *vl:%d\n",result,*vl);
		
	
	}
	va_end(vl);	
	return result;
	*/
	int i;
	void* p;
	p=&num;
	for(i=0;i<num;i++)
	{
		(int)p+=sizeof(int);
		printf("%d\n",*(int*)p);
	}
	return 1;
}
 
int main()
{
	 char* c="hello world";
	 int a=100;
     pthread_t pid;
     pthread_create(&pid, NULL, Function_t,&a);
	 pthread_create(&pid, NULL, Function_t,&a);
	 pthread_create(&pid, NULL, Function_t,&a);
	 pthread_create(&pid, NULL, Function_t,&a);
	 pthread_create(&pid, NULL, Function_t,&a);
     while (1)
     {
         printf("in fatherprocess!\n");
         Sleep(2000);
     }
     getchar();
     return 1;
	/*
	simple(5,1,2,3,4,5);
	return 1;
	*/
	
}
