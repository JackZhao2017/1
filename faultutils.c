#include "faultlog.h"
#include <errno.h>
#include "faultutils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void uartfaultlog(faultsign sign)
{
	char log[128];
	memset(log,0,sizeof(log));
	module_type module=0;
	switch(sign){
		case tty_uart_open:
			module = MODULE_UART;
			sprintf(log,"UART OPEN FAILED,%s",strerror(errno));
			break;
		case tty_uart_write:
			module = MODULE_UART;
			sprintf(log,"UART WRITE FAILED,%s",strerror(errno));
			break;
		case tty_uart_read:
			module = MODULE_UART;
			sprintf(log,"UART READ FAILED,%s",strerror(errno));	
			break;
		case tty_messager_timeout:
			module = MODULE_MESSAGE_R;
			sprintf(log,"MESSAGE TIMEOUT");	
			break;
		case tty_messager_err:
			module = MODULE_MESSAGE_R;
			sprintf(log,"MESSAGE ERR ,%s",strerror(errno));	
			break;
		case tty_messager_crc:
			module = MODULE_MESSAGE_R;
			sprintf(log,"MESSAGE CRC");	
			break;
		default :
			return;
			break;	
	}
	addlogtofile(LOG_FATAL,module,PORT_TTYMXC,sign,log);	
}


void  camerafaultlog(faultsign sign)
{
	char log[128];
	memset(log,0,sizeof(log));
	switch(sign)
	{	
		case camera_capture_dqbuf:
			sprintf(log,"CAMERA VIDIOC_DQBUF FAILED, %s",strerror(errno));
			break;
		case camera_capture_reqbufs:
			sprintf(log,"CAMERA VIDIOC_REQBUFS FAILED ,%s",strerror(errno)); 
			break;
		case camera_capture_query:
			sprintf(log,"CAMERA VIDIOC_QUERYBUF FAILED ,%s",strerror(errno));
			break;
		case camera_capture_mmap:
			sprintf(log,"CAMERA MMAP FAILED ,%s",strerror(errno));
			break;
		case camera_capture_qbuf:
			sprintf(log,"CAMERA VIDIOC_QBUF FAILED ,%s",strerror(errno));
			break;
		case camera_capture_on:
			sprintf(log,"CAMERA VIDIOC_STREAMON FAILED ,%s",strerror(errno)); 
			break;
		case camera_capture_open:
			sprintf(log,"CAMERA OPEN FAILED ,%s",strerror(errno));
			break;
		case camera_capture_operate:
			sprintf(log,"CAMERA OPERATE FAILED ,%s",strerror(errno));
			break;
		default :
			return;
			break;	
	}
	addlogtofile(LOG_SERIOUS,MODULE_CAPTURE,PORT_CAMERA,sign,log);
}


void sysfaultlog(faultsign sign)
{
	char log[128];
	memset(log,0,sizeof(log));
	module_type module=0;
	switch(sign){
		case sys_capture_malloc:
		    module = MODULE_CAPTURE;
		    break;
		case sys_messaget_malloc:
		case sys_messaget_mutex:
		case sys_messaget_thread:
		case sys_messaget_sem:
			module = MODULE_MESSAGE_T;
			break;
		case sys_messager_malloc:
		case sys_messager_thread:
			module = MODULE_MESSAGE_R;
			break;
		case sys_timer_signal:
		case sys_timer_setitimer:
		case sys_timer_sem:
			module = MODULE_TIMER;
			break;
		case sys_adasldw_thread:
		case sys_adasldw_sem:
		case sys_adasldw_mutex:
			module=MODULE_ADASLDW;
			break;
		case sys_adasfcw_thread:
		case sys_adasfcw_sem:
		case sys_adasfcw_mutex:
			module=MODULE_ADASFCW;
			break;			
		case sys_adassoft_cpuhot:
			module=MODULE_ADASSOFT;
			break;
		default:
			module = 0;
			break;
 	}
	switch(sign)
	{
		case sys_capture_malloc:
		case sys_messaget_malloc:
		case sys_messager_malloc:
			sprintf(log,"BUFFER MALLOC FAILED , %s",strerror(errno));
			break;
		case sys_adasldw_thread:
		case sys_adasfcw_thread:
		case sys_messager_thread:
		case sys_messaget_thread:
			sprintf(log,"THREAD CREATE FAILED, %s",strerror(errno));
			break;
		case  sys_timer_signal :
			sprintf(log,"TIMER SIGNAL FAILED , %s",strerror(errno));
			break;
		case  sys_timer_setitimer :
			sprintf(log,"TIMER SETITIMER FAILED , %s",strerror(errno));
			break;
		case  sys_adassoft_cpuhot :
			sprintf(log,"CPU TEMPERATURE HOT ");
			break;
		case  sys_timer_sem:
		case  sys_adasldw_sem:
		case  sys_adasfcw_sem:
		case  sys_messaget_sem:
			sprintf(log,"SEM INIT FAILED,%s",strerror(errno));
			break;
		case sys_messaget_mutex:
		case sys_adasldw_mutex:
		case sys_adasfcw_mutex:
			sprintf(log,"MUTEX INIT FAILED,%s",strerror(errno));
			break;
		default:
			return;
	}
	addlogtofile(LOG_SERIOUS,module,PORT_SYS,sign,log);
}

void adasfaultlog(faultsign sign ,char *file)
{
	char log[128];
	char data[2]="-1";
	memset(log,0,sizeof(log));
	module_type module=0;
	switch(sign){
		case adas_adassoft_init:
		case adas_adassoft_active:
		case adas_adassoft_file:
		case adas_adassoft_ky:
			module = MODULE_ADASSOFT;
			break;
		case adas_adasldw_ky:
			module = MODULE_ADASFCW;
			break;
		case adas_adasfcw_ky:
			module = MODULE_ADASFCW;
			break;
		default:
			module=0;
			break;
	}
	switch(sign)
	{
		case adas_adassoft_init:
			 sprintf(log,"ADAS INIT FAILED , %s",strerror(errno));
			 break;
		case adas_adassoft_active:
			sprintf(log,"ADAS ACTIVE FAILED");
			break;
		case adas_adasldw_ky:
		case adas_adasfcw_ky:
		case adas_adassoft_ky:
			sprintf(log,"ADAS KYCREATEIMAGE FAILED ,%s",strerror(errno));
			break;
		case adas_adassoft_file:
			if(!file){
				file=data;
			}
			sprintf(log,"ADAS FILE DOSE NO EXIST OR DAMAGE %s ,%s", file ,strerror(errno));
			break;
		default:
			return;
			break;
	}	
	addlogtofile(LOG_SERIOUS,module,PORT_ADAS,sign,log);
}

void  printfaultlogtest(faultsign sign)
{
	faultsign st,ed;
	st=start;
	ed=tty_end;
	printf("%s st %d ed %d sign %d\n",__func__,st,ed,sign );
	if((st<sign)&&(sign<ed)){
		uartfaultlog(sign);
		return;
	}
	st=sys_start;
	ed=sys_end;
	if((sign>st)&&(sign<ed)){
		sysfaultlog(sign);
		return;
	}
	st=camera_start;
	ed=camera_end;
	if((sign>st)&&(sign<ed)){
		camerafaultlog(sign);
		return;
	}
	st=adas_start;
	if(sign>adas_start){
		adasfaultlog(sign,NULL);
		return;
	}
}
int main(int argc, char const *argv[])
{
	faultsign sign=start;
	int i=0;
	for(i=0;i<46;i++)
	{
		sign=i;
		printfaultlogtest(sign);
	}
	return 0;
}




