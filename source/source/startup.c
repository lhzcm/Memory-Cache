#include<time.h>
#include<stdio.h>
#include"connection.h"
int Max_Connection;
int Now_Connection;
time_t session_time;

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

void ControlHandler(DWORD reauest);
void ServiceMain(int argc,char** argv);

int config_init()
{
	Max_Connection=4;
	Now_Connection=0;
	session_time=60;
	return 0;
}
int start()
{
	SOCKET sever_sockt;
	config_init();
	if(connection_init(&sever_sockt)==-1)//初始化socket连接
	{
		printf("init connection faile!\n");
	}
	start_run(sever_sockt);
	return 0;
}
int main(int argc,char* argv[])
{
	SERVICE_TABLE_ENTRY ServiceTable[2];
	ServiceTable[0].lpServiceName=L"MemoryCache";
	ServiceTable[0].lpServiceProc=(LPSERVICE_MAIN_FUNCTION)ServiceMain;

	ServiceTable[1].lpServiceName=NULL;
	ServiceTable[2].lpServiceProc=NULL;

	StartServiceCtrlDispatcher(ServiceTable);
	return 0;
}
void ServiceMain(int argc,char** argv)
{
	BOOL bRet;	
	int result;
	bRet = TRUE;


	ServiceStatus.dwServiceType=SERVICE_WIN32;
	ServiceStatus.dwCurrentState=SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted=SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN;

	ServiceStatus.dwWin32ExitCode=0;
	ServiceStatus.dwServiceSpecificExitCode=0;
	ServiceStatus.dwCheckPoint=0;
	ServiceStatus.dwWaitHint=0;

	hStatus=RegisterServiceCtrlHandler(L"MemoryCache",(LPHANDLER_FUNCTION)ControlHandler);
	if(hStatus==(SERVICE_STATUS_HANDLE)0)
	{
		return;
	}
	
	ServiceStatus.dwCurrentState=SERVICE_RUNNING;
	SetServiceStatus(hStatus,&ServiceStatus);

	while(ServiceStatus.dwCurrentState==SERVICE_RUNNING)
	{
		result=start();
		if(result<0)
			return;
	}
}
void ControlHandler(DWORD request)
{
	switch(request)
	{
		case SERVICE_CONTROL_STOP:
			ServiceStatus.dwWin32ExitCode=0;
			ServiceStatus.dwCurrentState=SERVICE_STOPPED;
			SetServiceStatus(hStatus,&ServiceStatus);
			return;
		case SERVICE_CONTROL_SHUTDOWN:
			ServiceStatus.dwWin32ExitCode = 0;
			ServiceStatus.dwCurrentState = SERVICE_STOPPED;
			SetServiceStatus(hStatus,&ServiceStatus);
			return;
		default:
			break;
	}
	SetServiceStatus(hStatus,&ServiceStatus);
	return;
}
