#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include "faultlog.h"

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
	memset(&buf,0,sizeof(buf));
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
			return "NORMAL     ";

		case LOG_RECOVERABLE:
			return "RECOVERABLE";

		case LOG_SERIOUS:
			return "SERIOUS    ";

		case LOG_FATAL:
			return "FATAL      ";

		case LOG_BEYONDRESPAIR:
			return "BEYONDRESPAIR";

		default:
			return "UNKNOWN";

	}
}
static char *getlog_port(int port)
{
	if(port==PORT_CAMERA)	{
		return "/dev/video0   ";
	}else if(port == PORT_TTYMXC){
		return "/dev/ttymxc1  ";
	}else if(port==PORT_ADAS){
		return "adas component";
	}else if(port==PORT_SYS){
		return "linux systerm ";
	}else{
		return "UNKNOWN";
	}
}

static char *getlog_module(int module)
{
	switch(module)
	{
		case MODULE_CAPTURE:
			return "capture ";
		case MODULE_UART:
			return "uart    ";
		case MODULE_ADASSOFT:
			return "adassoft";
		case MODULE_TIMER:
			return "timer   ";
		case MODULE_MESSAGE_R:
			return "messager";
		case MODULE_MESSAGE_T:
			return "messaget";
		case MODULE_ADASLDW:
			return "adasldw ";
		case MODULE_ADASFCW:
			return "adasfcw ";
		default: 
			return "unknown ";
	}	
}
FAULTLOG g_faultlog={.level=0,.module=0,.port=0,.count=0,.isflush=0};

static	char szLogcontent[128];
static 	char Logcontent[128];
static	char timer[56];

int pushlogtofile(const char *log){

	if(getfilesize(HWLOGFILEPATH)>MAXLOGFILESIZE){
		g_flog=fopen(HWLOGFILEPATH,"wt+");
	}else{
		g_flog=fopen(HWLOGFILEPATH,"at+");
	}	

	if(!g_flog){
		return -1;
	}

	fputs(log, g_flog);
	fclose(g_flog);
	return 0;
}


int addlogtofile(log_level level,module_type module,port_type port,int sign,char *des)
{
	char content[128];
	getlogtime(timer);
	FAULTLOG faultlog;
	if((g_faultlog.isflush==1)&&(g_faultlog.level==level)&&(g_faultlog.module==module)&&(g_faultlog.port==port)&&(g_faultlog.sign==sign)){
			g_faultlog.level=level;
			g_faultlog.module=module;
			g_faultlog.port=port;
			g_faultlog.sign=sign;
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
		g_faultlog.sign=sign;
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
		g_faultlog.sign=sign;
		memset(Logcontent,0,sizeof(Logcontent));
		memcpy(Logcontent,des,strlen(des));
	}
	
	if(faultlog.count>0){
		snprintf(szLogcontent,sizeof(szLogcontent)-1,"[%04d]",faultlog.count);
		pushlogtofile(szLogcontent);
	}

	snprintf(szLogcontent,sizeof(szLogcontent)-1,"\n[%s][%s][%s][%s][%s]",
		      getlog_level(g_faultlog.level),getlog_module(g_faultlog.module),getlog_port(g_faultlog.port),timer,Logcontent 
	);
	pushlogtofile(szLogcontent);
	return 0;
}

void flushlogtofile()
{
	snprintf(szLogcontent,sizeof(szLogcontent)-1,"[%04d]\n",g_faultlog.count);
	pushlogtofile(szLogcontent);
	memcpy(&g_faultlog,&g_faultlog,sizeof(g_faultlog));
}



