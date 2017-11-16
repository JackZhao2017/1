#ifndef _FAULTLOG_H_
#define _FAULTLOG_H_

//level
#define 	NORMAL 			"NORMAL       "
#define 	RECOVERABLE		"RECOVERABLE  "
#define		SERIOUS			"SERIOUS      "
#define		FATAL			"FATAL        "
#define		BEYONDRESPAIR	"BEYONDRESPAIR"
#define		UNKNOWN         "UNKNOWN      "

#define     HWLOGFILEPATH 	"/home/root/adas/hwlog.log"

//port name
#define     CAMERA  		"/dev/video0 "
#define 	UART 			"/dev/ttymxc1"
#define     CPU   			"cpu         "
#define     ADAS 			"adas        "
#define 	LINUX           "linux       "

//modules name 	
#define		ADASCAPTURE		"adas capture"
#define     ADASUART 	    "adas uart   "
#define     ADASCPUTEMP		"adas cputemp"
#define     ADASINIT 		"adas init   "
#define    	ADASACTIVE      "adas active "
#define     ADASFILE   		"adas file   "
#define 	LINUX_SYS       "linux sys   "
#define 	ADASTIMER       "adas timer  "


typedef 	int log_level;
#define     LOG_NORMAL			1	
#define     LOG_RECOVERABLE		2
#define	    LOG_SERIOUS			3
#define	    LOG_FATAL			4
#define	    LOG_BEYONDRESPAIR 	5

typedef     int module_type;
#define     MODULE_ADASCAPTURE  1
#define		MODULE_ADASUART		2
#define	    MODULE_ADASCPU      3
#define 	MODULE_ADASINIT		4
#define 	MODULE_ADASACTIVE	5
#define     MODULE_SYS 			6
#define     MODULE_ADASFILE     7
#define     MODULE_TIMER		8

typedef     int port_type;
#define     PORT_CAMERA			1
#define     PORT_UART			2
#define     PORT_CPU			3
#define     PORT_FILE			4
#define     PORT_ADAS			5
#define     PORT_SYS			6

typedef struct{
	int level;
	int module;
	int port;
	int count;
	int isflush;
}FAULTLOG;

//int addlogtofile(FAULTLOG *log);
int addlogtofile(log_level level,module_type module,port_type port,char * des);
void  flushlogtofile(module_type module,port_type port);
#endif