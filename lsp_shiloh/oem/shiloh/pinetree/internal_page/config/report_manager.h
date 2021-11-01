#ifndef REPORT_MANAGER_H
#define REPORT_MANAGER_H

typedef struct {
	char Time[12];
	char S_R[4];
	char FCFData[60];
	char FIFData[60];
	char FIFData2[60];
} T30_LOG_REPORT_t;

typedef struct {
    char Speed[15];
	char Resolution[10];
	char Mode[10];
	char Coding[5];
	char Result[20];
} T30_LOG_RESULT_t;

void network_report(void);
void report_manager_init(void);

#endif
