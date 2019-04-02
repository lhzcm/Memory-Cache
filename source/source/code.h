#include "dataTree.h"
#ifndef _CODE_H_
#define _CODE_H_
typedef struct Code
{
	unsigned short code;
	unsigned int key;
	char* msg;
	char* data;
}Code;

//static char* code_template="{\"Code\":\"%d\";\"Msg\":\"%s\"}";
static char* NULL_CHAR="NULL";
static const char* code_template="{\"Code\":%d,\"Key\":%d,\"Data\":\"%s\",\"Msg\":\"%s\"}\0";

static Code code_201={(unsigned short)201,0,&"socket connection successful!",NULL}; //socket连接成功

static Code code_400={(unsigned short)400,0,&"socket connection error!",NULL}; //socket连接出错
static Code code_401={(unsigned short)401,0,&"socket read data error!",NULL}; //socket读取数据出错
static Code code_404={(unsigned short)404,0,&"get faile,not found data by key!",NULL};
static Code code_405={(unsigned short)405,0,&"delete faile,not found data by key!",NULL};
static Code code_406={(unsigned short)406,0,&"has max connection plase wait!",NULL};

static Code code_500={(unsigned short)500,0,&"command syntax error!",NULL}; //命令语法错误
static Code code_501={(unsigned short)501,0,&"memory malloc error!",NULL};
static Code code_502={(unsigned short)502,0,&"key error!",NULL};
static Code code_503={(unsigned short)503,0,&"expirt time error!",NULL}; //过期时间有误
static Code code_504={(unsigned short)504,0,&"data has error!",NULL}; 
#endif
