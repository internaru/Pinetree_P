#include <stdio.h>
#include "agMessage.h"
#include "lassert.h"
#include "posix_ostools.h"


#define SDH_FAX_APP_STACK_SIZE  (1024 * 10 * 3) //POSIX_MIN_STACK_SIZE    ///< Size of the stack: 8K


extern void *fipp_main_thread (void* unused);


static pthread_t sdhfaxapp_thd_id;
static unsigned char SDHFaxAppStack[SDH_FAX_APP_STACK_SIZE] ALIGN8;


void faxapp_main_init (void)
{
    int retcode = 0;

    printf("\n(YHCHO) Welcome To The World of Pinetree Fax!\n");

    retcode = posix_create_thread(&sdhfaxapp_thd_id, fipp_main_thread, 0, "tSDHFaxApp", SDHFaxAppStack, SDH_FAX_APP_STACK_SIZE, POSIX_THR_PRI_NORMAL);

    XASSERT( retcode==0, retcode );
}

