typedef struct Code
{
	unsigned short code;
	unsigned int key;
	char* msg;
}Code;
char* code_template="{\"Code\":\"%d\";\"Msg\":\"%s\"}";
Code code_201={(unsigned short)201,0,&"socket���ӳɹ�"};

Code code_400={(unsigned short)400,0,&"socket���ӳ���"};
Code code_401={(unsigned short)401,0,&"socket��ȡ���ݳ���"};
Code code_402={(unsigned short)402,0,&"����ʱ������"};

Code code_500={(unsigned short)500,0,&"�﷨����"};
