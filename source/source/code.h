#ifndef _CODE_H_
#define _CODE_H_
typedef struct Code
{
	unsigned short code;
	unsigned int key;
	char* msg;
}Code;

static char* code_template="{\"Code\":\"%d\";\"Msg\":\"%s\"}";
static Code code_201={(unsigned short)201,0,&"socket���ӳɹ�"};

static Code code_400={(unsigned short)400,0,&"socket���ӳ���"};
static Code code_401={(unsigned short)401,0,&"socket��ȡ���ݳ���"};
static Code code_402={(unsigned short)402,0,&"����ʱ������"};

static Code code_500={(unsigned short)500,0,&"�﷨����"};

#endif
