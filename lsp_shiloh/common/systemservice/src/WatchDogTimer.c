/*
 ***********************************************************
 *(c) Copyright Sindoh Co. Ltd. 
 *
 *               SINDOH Confidential
 * 
 ***********************************************************
 **/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "logger.h"
#include "asic.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#undef DBG_SOFT
#define DBG_SOFT LOG_DEBUG

static int	fd_wd = 0;
static char *map_base_wd = NULL;
#define maddr(reg)      (unsigned long)(map_base_wd + (reg))
#define reg(reg)        (*((volatile unsigned int *)maddr(reg)))

#define	REG_TWR0					0x00
#define	REG_TTCR0					0x04
#define	REG_TCR0					0x08
#define	REG_TSR0					0x0C
#define	WD_REG_MAX					0x10

#define	WD_CMD_INIT					0x0
#define	WD_CMD_ENABLE				0x1
#define	WD_CMD_DISABLE				0x2
#define	WD_CMD_SERVICE				0x3
#define	WD_CMD_VIEW					0x4
#define	WD_CMD_SET_TERMINAL_COUNT	0x5

void watchdog_init(int terminal_count);
void watchdog_exit(void);
void watchdog_disable(void);
void watchdog_service(void);
void watchdog_set_terminal_count(int ms);
void watchdog_cmd(char *str);

static void watchdog_run(int cmd);
static void watchdog_run_para(int cmd, int para);

void watchdog_init(int terminal_count)
{
	fd_wd = open("/dev/mem", O_RDWR|O_NDELAY);
	if (fd_wd == -1){ 
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("/dev/mem open error"));
		return;
	}    
	map_base_wd = (char*)mmap(NULL, WD_REG_MAX, PROT_READ | PROT_WRITE, MAP_SHARED, fd_wd, APB_TMR_BASE);

	if (map_base_wd == MAP_FAILED){
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("/dev/mem open error"));
		close(fd_wd);
		return;
	}
	watchdog_run_para(WD_CMD_INIT, terminal_count);
}

void watchdog_exit(void){
	watchdog_run(WD_CMD_DISABLE);
//	DPRINTF(DBG_SOFT|DBG_OUTPUT, ("TCR0: %xh %xh\n", TCR0, reg(TCR0)));//, timer_regs->TCR0));
	if ( munmap(map_base_wd, WD_REG_MAX) == -1 ) {
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("/dev/mem open error"));
	}
	close(fd_wd);
	fd_wd = 0;
	return;
}

void watchdog_disable(void){
	watchdog_run(WD_CMD_DISABLE);
}

void watchdog_service(void){
	watchdog_run(WD_CMD_SERVICE);
}

void watchdog_set_terminal_count(int ms){
	watchdog_run_para(WD_CMD_SET_TERMINAL_COUNT, ms);
}

static void watchdog_run(int cmd) {
	watchdog_run_para(cmd, 0);
}

static void watchdog_run_para(int cmd, int para) {
	DPRINTF(DBG_SOFT|DBG_OUTPUT, ("WD_RUN: %d\n", cmd));
	int	value = 0;
	int invert = 0;
	if (fd_wd <= 0 ){
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("watchdog not initialized"));
		return;
	}
	switch(cmd) {
	case WD_CMD_INIT:
		value = TIMERS_TTCR0_TERMINALCOUNT_REPLACE_VAL(value, para/100);// unit: 100ms
		reg(REG_TTCR0) = value;

		value = reg(REG_TCR0);
		value = TIMERS_TCR0_TIMEBASESEL_REPLACE_VAL(value, 5);// Timebase Input: 100ms
		value = TIMERS_TCR0_CONTMODE_REPLACE_VAL(value, 0x1);	// Continuous mode
		value = TIMERS_TCR0_ENABLE_REPLACE_VAL(value, 1);		// Enable Timer0
		reg(REG_TCR0) = value;

		watchdog_run(WD_CMD_ENABLE);
		break;
	case WD_CMD_ENABLE:
		value = reg(REG_TWR0);
		value = TIMERS_TWR0_ENABLE_REPLACE_VAL(value, 1);       // Enable watchdog mode
		reg(REG_TWR0) = value;
		value = reg(REG_TCR0);
		value = TIMERS_TCR0_ENABLE_REPLACE_VAL(value, 1);		// Enable Timer0
		reg(REG_TCR0) = value;
		break;
	case WD_CMD_DISABLE:
		value = reg(REG_TWR0);
		value = TIMERS_TWR0_SERVICE_REPLACE_VAL(value, (0x0F^(TIMERS_TWR0_SERVICE_MASK_SHIFT(value))) );
		value = TIMERS_TWR0_ENABLE_REPLACE_VAL(value, 0);       // Enable watchdog mode
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("DIS:	%xh\n", value));
		reg(REG_TWR0) = value;
		value = reg(REG_TCR0);
		value = TIMERS_TCR0_ENABLE_REPLACE_VAL(value, 0);		// Disable Timer0
		reg(REG_TCR0) = value;
		break;
	case WD_CMD_SERVICE:
		value = reg(REG_TWR0);
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("TWR0:	%xh\n", value));
		invert = 0x0F ^ (TIMERS_TWR0_SERVICE_MASK_SHIFT(value));
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("invert:	%xh\n", invert));
		value = TIMERS_TWR0_SERVICE_REPLACE_VAL(value, invert);
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("TWR0:	%xh\n", value));
		reg(REG_TWR0) = value;
		break;
	case WD_CMD_SET_TERMINAL_COUNT:
		watchdog_run(WD_CMD_VIEW);
		watchdog_run(WD_CMD_DISABLE);
		watchdog_run(WD_CMD_VIEW);
		value = TIMERS_TTCR0_TERMINALCOUNT_REPLACE_VAL(value, para/100);// unit: 100ms
		reg(REG_TTCR0) = value;
		watchdog_run(WD_CMD_VIEW);
		watchdog_run(WD_CMD_ENABLE);
		watchdog_run(WD_CMD_VIEW);
		break;
	case WD_CMD_VIEW:
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("TWR0:	%xh\n", reg(REG_TWR0)));
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("TTCR0:	%xh\n", reg(REG_TTCR0)));
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("TCR0:	%xh\n", reg(REG_TCR0)));
		DPRINTF(DBG_SOFT|DBG_OUTPUT, ("TSR0:	%xh\n", reg(REG_TSR0)));
		break;
	}
}

extern void check_segmentation_fault(char *str, int line);
void check_segmentation_fault(char *str, int line){
	FILE *fp;
	int size = 0;
#define BUF_SIZE    (32)
	char buf[BUF_SIZE]={0};

	DPRINTF(LOG_ERR|DBG_OUTPUT, ("seg: %s, %d\n", str, line));
	fp = popen("echo 1", "r");
	if(fp != NULL){
		size = fread(buf, sizeof(char), sizeof(buf), fp);
		buf[BUF_SIZE-1]=0;
		if (size == 0 || buf[0] != '1'){
			DPRINTF(LOG_ERR|DBG_OUTPUT, ("segmentation fault detected\n"));
			DPRINTF(LOG_ERR|DBG_OUTPUT, ("size: %d, buf: %s\n", size, buf));
			watchdog_init(200);
			sleep(1);
		}   
		DPRINTF(LOG_ERR|DBG_OUTPUT, ("seg check: size %d\nbuf %s\n", size, buf));
		pclose(fp);
	}   
	else {
		DPRINTF(LOG_ERR|DBG_OUTPUT, ("%s error\n", __FUNCTION__));
	}   
}

void watchdog_cmd(char *str)
{
	DPRINTF(DBG_SOFT|DBG_OUTPUT, ("wdt %s %d, %s\n", __func__, __LINE__, str));

    if ( strncmp(str, "in", 2 ) == 0 ){
		watchdog_init(10000);
	}
    else if (strncmp(str, "en", 2) == 0) {
		watchdog_run(WD_CMD_ENABLE);
	}
    else if (strncmp(str, "di", 2) == 0) {
		watchdog_run(WD_CMD_DISABLE);
	}
    else if (strncmp(str, "se", 2) == 0) {
		watchdog_run(WD_CMD_SERVICE);
	}
    else if (strncmp(str, "vi", 2) == 0) {
		watchdog_run(WD_CMD_VIEW);
	}
    else if (strncmp(str, "tc5", 3) == 0) {
		watchdog_run_para(WD_CMD_SET_TERMINAL_COUNT, 5000);
	}
    else if (strncmp(str, "tc10", 4) == 0) {
		watchdog_run_para(WD_CMD_SET_TERMINAL_COUNT, 10000);
	}
    else if (strncmp(str, "tc20", 4) == 0) {
		watchdog_run_para(WD_CMD_SET_TERMINAL_COUNT, 20000);
	}
}
