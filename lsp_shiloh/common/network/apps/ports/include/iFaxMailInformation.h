#ifndef IFAXMAILINFORMATION_H_
#define IFAXMAILINFORMATION_H_
#ifdef __cplusplus
extern "C" {
#endif
#define MAX_IFAX_STR 1024
typedef struct {
	char *iFaxUserName;
	char *iFaxEMail;
	char *iFaxReplyEMail;
} iFaxUserInfomation_t;

typedef struct{
	char *sendMailAddress;
	int sendMailPort;
	int sendMailAuthentiCation;
	int sendMailSecurity;
	char sendMailUSer;
	char *sendMailPassword;
} iFaxSendInformation_t;

typedef struct{
	int receiveMailType;
	char receiveMailAddress[MAX_IFAX_STR];
	int receiveMailPort;
	int receiveMailAuthentication;
	int receiveMailSecurity;
	char receiveMailUser[MAX_IFAX_STR];
	char receiveMailPassword[MAX_IFAX_STR];
	int checkInterval;
} iFaxReceiveInformation_t;

int getIFaxUserInformation(iFaxUserInfomation_t *iFaxUserInformation);
int getIFaxSendInformation(iFaxSendInformation_t *iFaxSendInformation);
int getIFaxReceiveInformation();

#ifdef __cplusplus
}
#endif
#endif

