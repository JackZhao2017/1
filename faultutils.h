#ifndef _FAULTUTILS_H_
#define _FAULTUTILS_H_

typedef enum _faultsign
{
	start=0,           
	tty_uart_open,
	tty_uart_write,
	tty_uart_read,
	tty_messager_timeout,
	tty_messager_err,
	tty_messager_crc,
	tty_end,   //7

	sys_start,
	sys_messager_thread,
	sys_messager_mutex,
	sys_messager_malloc,

	sys_messaget_thread,
	sys_messaget_sem,
	sys_messaget_mutex,
	sys_messaget_malloc,

	sys_capture_malloc,

	sys_timer_signal,
	sys_timer_setitimer,
	sys_timer_sem,

	sys_adasldw_thread,
	sys_adasldw_sem,
	sys_adasldw_mutex,
	sys_adasfcw_thread,
	sys_adasfcw_sem,
	sys_adasfcw_mutex,

	sys_adassoft_cpuhot,
	sys_end,  

	camera_start,  //29
	camera_capture_open,
	camera_capture_dqbuf,
	camera_capture_qbuf,
	camera_capture_reqbufs,
	camera_capture_query,
	camera_capture_mmap,
	camera_capture_on,
	camera_capture_operate,
	camera_end,

	adas_start,
	adas_adassoft_active,
	adas_adassoft_init,
	adas_adassoft_file,
	adas_adassoft_ky,
	adas_adasldw_ky,
	adas_adasfcw_ky,

	end
}faultsign;



void sysfaultlog(faultsign sign);
void adasfaultlog(faultsign sign ,char *file);
void camerafaultlog(faultsign sign);
void uartfaultlog(faultsign sign);
#endif