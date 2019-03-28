typedef struct Code
{
	unsigned short code;
	unsigned int key;
	char* msg;
}Code;
char* code_template="{\"Code\":\"%d\";\"Msg\":\"%s\"}";
Code code_201={(unsigned short)201,0,&"socket连接成功"};

Code code_400={(unsigned short)400,0,&"socket连接出错"};
Code code_401={(unsigned short)401,0,&"socket读取数据出错"};
Code code_402={(unsigned short)402,0,&"过期时间有误"};

Code code_500={(unsigned short)500,0,&"语法错误"};
