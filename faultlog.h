#ifndef _FAULTLOG_H_
#define _FAULTLOG_H_
#define     HWLOGFILEPATH 	"./hwlog.log"

typedef 	int log_level;
#define     LOG_NORMAL			1	
#define     LOG_RECOVERABLE		2
#define	    LOG_SERIOUS			3
#define	    LOG_FATAL			4
#define	    LOG_BEYONDRESPAIR 	5

typedef     int module_type;

#define     MODULE_CAPTURE  	1
#define		MODULE_UART			2
#define	    MODULE_MESSAGE_R    3
#define 	MODULE_ADASSOFT		4
#define 	MODULE_ADASLDW		5
#define 	MODULE_ADASFCW		6
#define 	MODULE_TIMER  		7
#define	    MODULE_MESSAGE_T    8

typedef     int port_type;
#define     PORT_CAMERA			1
#define     PORT_TTYMXC			2
#define     PORT_ADAS			3
#define     PORT_SYS			4

typedef struct{
	int level;
	int module;
	int port;
	int count;
	int isflush;
	int sign;
}FAULTLOG;
int addlogtofile(log_level level,module_type module,port_type port,int sign, char * des);
void  flushlogtofile(void);
#endif