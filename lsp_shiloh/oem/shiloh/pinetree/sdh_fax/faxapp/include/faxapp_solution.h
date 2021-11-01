#ifndef __FAXAPP_SOLUTION__
#define __FAXAPP_SOLUTION__


#include "posix_ostools.h"

#define FAXSOLUTIONTASK_QUEUE_SIZE     500

#define FAXAPP_SOLUTION_STACK_SIZE (MAX((1024 * 10 * 3), POSIX_MIN_STACK_SIZE))


void faxapp_solution_request_send (int jnum);
void faxapp_solutioin_send_to_email (int jnum);
void faxapp_solutioin_send_to_ftp(int jnum);
#if __HAVE_FAXSOLUTION_DK__
void faxapp_solution_send_to_ftp_dk (char *path, UINT16 jnum);
#endif
void faxapp_solutioin_send_to_smb(int jnum);
void faxapp_solutioin_send_to_webdav(int jnum);

extern void set_txq_nxt (void);
extern void fsu_delete_dir_on_file_system (const char *Path);
extern void fsu_delete_file_on_file_system (const char *Path);
extern void fsu_request_status (int jnum, int status);
extern void fsu_update_txq_nxt (int jnum);

#endif // #ifndef __FAXAPP_SOLUTION__
