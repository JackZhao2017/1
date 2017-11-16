#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include "faultlog.h"
#include "printlog.h"

#define MAXLOGFILESIZE 4194304

FILE *g_flog=NULL;

static long long currenttime(){
    struct timeval now;
    gettimeofday(&now, NULL);
    long long when = now.tv_sec * 1000LL + now.tv_usec / 1000;
    return when;
}
static int getfilesize(const char *file )
{
	struct stat buf;
	stat(file,&buf); 
	return	 buf.st_size;
}
static int getlogtime(char *ti)
{
	 time_t t; 
	 struct tm *local;
	 t=time(NULL);
	 local= localtime(&t);
	 sprintf(ti,"%d-%02d-%02d-%02d-%02d-%02d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
	 return 0;
}
static char *getlog_level(int level)
{
	switch(level){

		case LOG_NORMAL: 
			return NORMAL;

		case LOG_RECOVERABLE:
			return RECOVERABLE;

		case LOG_SERIOUS:
			return SERIOUS;

		case LOG_FATAL:
			return FATAL;

		case LOG_BEYONDRESPAIR:
			return BEYONDRESPAIR;

		default:
			return UNKNOWN;

	}
}

static char *getlog_port(int port)
{
	if(port==PORT_CAMERA)	{
		return CAMERA;
	}else if(port == PORT_UART){
		return UART;
	}else if(port==PORT_CPU){
		return CPU;
	}else if(port==PORT_ADAS){
		return ADAS;
	}else if(port==PORT_FILE){
		return ADAS;
	}else if(port==PORT_SYS){
		return LINUX;
	}else{
		return UNKNOWN;
	}

}

static char *getlog_module(int module)
{
	switch(module)
	{
		case MODULE_ADASCAPTURE:
			return ADASCAPTURE ;
		case MODULE_ADASUART:
			return ADASUART ;
		case MODULE_ADASCPU:
			return ADASCPUTEMP;
		case MODULE_ADASACTIVE:
			return ADASACTIVE;
		case MODULE_ADASINIT:
			return ADASINIT;
		case MODULE_SYS:
			return LINUX_SYS;
		case MODULE_ADASFILE:
			return ADASFILE;
		case MODULE_TIMER:
			return ADASTIMER;
		default: 
			return UNKNOWN;
	}	
}
// int faultlogInit(void)
// {
// 	g_flog=fopen(HWLOGFILEPATH,"at+");
// 	if(!g_flog){
// 		printf("%s failed\n",__func__ );

// 	}
// }

// void faultlogRelease(void)
// {
// 	fclose(g_flog);
// }
int pushlogtofile(const char *log){

	if(getfilesize(HWLOGFILEPATH)>MAXLOGFILESIZE){
		g_flog=fopen(HWLOGFILEPATH,"wt+");
	}else{
		g_flog=fopen(HWLOGFILEPATH,"at+");
	}

	
	if(!g_flog){
		err_msg("%s failed\n",__func__ );
		return -1;
	}

	fputs(log, g_flog);

	fclose(g_flog);
	return 0;
}

FAULTLOG g_faultlog={.level=0,.module=0,.port=0,.count=0,.isflush=0};
static	char szLogcontent[128];
static 	char Logcontent[128];
static	char timer[56];


int addlogtofile(log_level level,module_type module,port_type port,char *des)
{
	char content[128];
	getlogtime(timer);
	FAULTLOG faultlog;
	int statuing=0;
	if((g_faultlog.isflush==1)&&(g_faultlog.level==level)&&(g_faultlog.module==module)&&(g_faultlog.port==port)){
			g_faultlog.level=level;
			g_faultlog.module=module;
			g_faultlog.port=port;
			memset(Logcontent,0,sizeof(Logcontent));			
			memcpy(Logcontent,des,strlen(des));
			if(g_faultlog.count<9999){
				++g_faultlog.count;
			}
			return 0;
	}else if(g_faultlog.isflush==0){      
		memset(content,0,sizeof(content));
		memcpy(&faultlog,&g_faultlog,sizeof(g_faultlog));
		memcpy(content,des,strlen(Logcontent));
		g_faultlog.level=level;
		g_faultlog.module=module;
		g_faultlog.port=port;
		++g_faultlog.count;	
		g_faultlog.isflush=1;
		memset(Logcontent,0,sizeof(Logcontent));	
		memcpy(Logcontent,des,strlen(des));
	}else{
		memset(content,0,sizeof(content));
		memcpy(content,Logcontent,strlen(Logcontent));
		memcpy(&faultlog,&g_faultlog,sizeof(g_faultlog));
		g_faultlog.count=1;
		g_faultlog.level=level;
		g_faultlog.module=module;
		g_faultlog.port=port;
		memset(Logcontent,0,sizeof(Logcontent));
		memcpy(Logcontent,des,strlen(des));
	}
	if(faultlog.count>1){
		snprintf(szLogcontent,sizeof(szLogcontent)-1,"[%s][%s][%s][%s][%04d][%s]\n",
		      getlog_level(faultlog.level),getlog_module(faultlog.module),getlog_port(faultlog.port),timer,faultlog.count,content 
			);

		pushlogtofile(szLogcontent);
	}

	if(statuing){
		snprintf(szLogcontent,sizeof(szLogcontent)-1,"[%s][%s][%s][%s][*ing][%s]\n",
		      getlog_level(g_faultlog.level),getlog_module(g_faultlog.module),getlog_port(g_faultlog.port),timer,Logcontent 
			);
	}else{
		snprintf(szLogcontent,sizeof(szLogcontent)-1,"[%s][%s][%s][%s][%04d][%s]\n",
		      getlog_level(g_faultlog.level),getlog_module(g_faultlog.module),getlog_port(g_faultlog.port),timer,g_faultlog.count,Logcontent 
			);
	}
	pushlogtofile(szLogcontent);


	return 0;
}

void  flushlogtofile(module_type module,port_type port)
{
	if(g_faultlog.isflush==0)
		return ;
	if(g_faultlog.count==1)
		return ;
	if(((g_faultlog.module!=module)||(g_faultlog.port!=port))&&module&&port)
		return;
	snprintf(szLogcontent,sizeof(szLogcontent)-1,"[%s][%s][%s][%s][%04d][%s]\n",
		      getlog_level(g_faultlog.level),getlog_module(g_faultlog.module),getlog_port(g_faultlog.port),timer,g_faultlog.count,Logcontent 
			);

	pushlogtofile(szLogcontent);
	memcpy(&g_faultlog,&g_faultlog,sizeof(g_faultlog));
}



