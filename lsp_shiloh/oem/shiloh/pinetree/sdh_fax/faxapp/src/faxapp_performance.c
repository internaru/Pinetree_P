#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include "SDHFaxCommonHeader.h"


#if __MEASURE_PERFORMANCE__

/* T30 Direction */
#define T30_RECEIVE     0
#define T30_TRANSMIT     1
#define MEM_TO_PRINT     2
#define SCAN_TO_MEM     3
#define T30_TRANSMIT_BOTTLE_NECK     4
#define ON_HOOK     5


typedef struct
{
    unsigned long StartInSec;
    unsigned long StartInUSec;
    unsigned long EndInSec;
    unsigned long EndInUSec;
} PERFORMACE_CHECK_t;


PERFORMACE_CHECK_t Scan;
PERFORMACE_CHECK_t Transmit;
PERFORMACE_CHECK_t Receive;
PERFORMACE_CHECK_t Print;
PERFORMACE_CHECK_t BottleNeck;
PERFORMACE_CHECK_t OnHook;


char String[64] = {0, };


void faxapp_get_job_start_time (int JobType)
{
    struct timeval Start;

    gettimeofday(&Start, NULL);

    switch (JobType)
    {
        case SCAN_TO_MEM:
            Scan.StartInSec = Start.tv_sec;
            Scan.StartInUSec = Start.tv_usec;
            break;

        case T30_TRANSMIT:
            Transmit.StartInSec = Start.tv_sec;
            Transmit.StartInUSec = Start.tv_usec;
            break;

        case T30_RECEIVE:
            Receive.StartInSec = Start.tv_sec;
            Receive.StartInUSec = Start.tv_usec;
            break;

        case MEM_TO_PRINT:
            Print.StartInSec = Start.tv_sec;
            Print.StartInUSec = Start.tv_usec;
            break;

        case T30_TRANSMIT_BOTTLE_NECK:
            BottleNeck.StartInSec = Start.tv_sec;
            BottleNeck.StartInUSec = Start.tv_usec;
            break;

        case ON_HOOK:
            OnHook.StartInSec = Start.tv_sec;
            OnHook.StartInUSec = Start.tv_usec;
            break;

        default:
            break;
    }
}


void faxapp_get_job_end_time (int JobType)
{
    struct timeval End;

    gettimeofday(&End, NULL);

    switch (JobType)
    {
        case SCAN_TO_MEM:
            Scan.EndInSec = End.tv_sec;
            Scan.EndInUSec = End.tv_usec;
            break;

        case T30_TRANSMIT:
            Transmit.EndInSec = End.tv_sec;
            Transmit.EndInUSec = End.tv_usec;
            break;

        case T30_RECEIVE:
            Receive.EndInSec = End.tv_sec;
            Receive.EndInUSec = End.tv_usec;
            break;

        case MEM_TO_PRINT:
            Print.EndInSec = End.tv_sec;
            Print.EndInUSec = End.tv_usec;
            break;

        case T30_TRANSMIT_BOTTLE_NECK:
            BottleNeck.EndInSec = End.tv_sec;
            BottleNeck.EndInUSec = End.tv_usec;
            break;

        case ON_HOOK:
            OnHook.EndInSec = End.tv_sec;
            OnHook.EndInUSec = End.tv_usec;
            break;

        default:
            break;
    }
}


char *faxapp_get_elapsed_time (int JobType)
{
    long DiffInSec = 0;
    long DiffInUSec = 0;

    memset(String, 0, sizeof(char));

    switch (JobType)
    {
        case SCAN_TO_MEM:
        {
            DiffInSec = Scan.EndInSec - Scan.StartInSec;
            DiffInUSec = Scan.EndInUSec - Scan.StartInUSec;

            if (DiffInUSec < 0)
            {
                --DiffInSec;

                DiffInUSec += 1000000;
            }

            snprintf(String, sizeof(String), "Elapsed Time [SCAN TO MEM]: %ld.%06ld", DiffInSec, DiffInUSec);

            break;
        }

        case T30_TRANSMIT:
        {
            DiffInSec = Transmit.EndInSec - Transmit.StartInSec;
            DiffInUSec = Transmit.EndInUSec - Transmit.StartInUSec;

            if (DiffInUSec < 0)
            {
                --DiffInSec;

                DiffInUSec += 1000000;
            }

            snprintf(String, sizeof(String), "Elapsed Time [MEM TO SEND]: %ld.%06ld", DiffInSec, DiffInUSec);

            break;
        }

        case T30_RECEIVE:
        {
            DiffInSec = Receive.EndInSec - Receive.StartInSec;
            DiffInUSec = Receive.EndInUSec - Receive.StartInUSec;

            if (DiffInUSec < 0)
            {
                --DiffInSec;

                DiffInUSec += 1000000;
            }

            snprintf(String, sizeof(String), "Elapsed Time [RECEIVE TO MEM]: %ld.%06ld", DiffInSec, DiffInUSec);

            break;
        }

        case MEM_TO_PRINT:
        {
            DiffInSec = Print.EndInSec - Print.StartInSec;
            DiffInUSec = Print.EndInUSec - Print.StartInUSec;

            if (DiffInUSec < 0)
            {
                --DiffInSec;

                DiffInUSec += 1000000;
            }

            snprintf(String, sizeof(String), "Elapsed Time [MEM TO PRINT]: %ld.%06ld", DiffInSec, DiffInUSec);

            break;
        }

        case T30_TRANSMIT_BOTTLE_NECK:
        {
            DiffInSec = BottleNeck.EndInSec - BottleNeck.StartInSec;
            DiffInUSec = BottleNeck.EndInUSec - BottleNeck.StartInUSec;

            if (DiffInUSec < 0)
            {
                --DiffInSec;

                DiffInUSec += 1000000;
            }

            snprintf(String, sizeof(String), "Elapsed Time [MEM TO SEND BOTTLE NECK]: %ld.%06ld", DiffInSec, DiffInUSec);

            break;
        }

        case ON_HOOK:
        {
            DiffInSec = OnHook.EndInSec - OnHook.StartInSec;
            DiffInUSec = OnHook.EndInUSec - OnHook.StartInUSec;

            if (DiffInUSec < 0)
            {
                --DiffInSec;

                DiffInUSec += 1000000;
            }

            snprintf(String, sizeof(String), "Elapsed Time [ON HOOK]: %ld.%06ld", DiffInSec, DiffInUSec);

            break;
        }

        default:
        {
            printf("\n(YHCHO) %s() - Invalid JobType: %d\n", __FUNCTION__, JobType);

            break;
        }
    } // switch (JobType)

    return String;
}
#endif // #if __MEASURE_PERFORMANCE__

