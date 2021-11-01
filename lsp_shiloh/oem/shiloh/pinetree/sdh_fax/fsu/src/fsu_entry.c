#include "SDHFaxCommonHeader.h"
#include "posix_ostools.h"
#include "lassert.h"


int gFaxTxCompletePageNo = 0;

extern void fsu_set_event_flags_tx_complete (void);
extern void fsu_set_event_flags_rx_complete (void);
extern void *fsu_main (void* unused);


#define FSU_ENTRY_STACK_SIZE     (1024 * 10 * 3)


static unsigned char SDHFaxFSUStack[FSU_ENTRY_STACK_SIZE];
static pthread_t sdhfaxfsu_thd_id;


void fsu_process_start( void )
{
    int retcode = 0;

    retcode = posix_create_thread(&sdhfaxfsu_thd_id, fsu_main, 0, "tSDHFaxFSU", SDHFaxFSUStack, FSU_ENTRY_STACK_SIZE, POSIX_THR_PRI_NORMAL);

    XASSERT( retcode==0, retcode );
}


void faxapp_set_event_flags (int Event)
{
    switch (Event)
    {
        case FAXAPP_T30_EVENT_TX_COMPLETE:
            gFaxTxCompletePageNo++;
            fsu_set_event_flags_tx_complete();
            break;

        case FAXAPP_T30_EVENT_RX_COMPLETE:
            fsu_set_event_flags_rx_complete();
            break;

        default:
            break;
    }
}

