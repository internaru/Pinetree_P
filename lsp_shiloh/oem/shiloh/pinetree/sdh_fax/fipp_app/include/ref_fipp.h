/******************************************************************************
**                                                                           **
**                 Proprietary and Confidential Information                  **
**                                                                           **
**                 Copyright (c) SigmaTel, Inc. Unpublished                  **
**                                                                           **
** This source code and the algorithms implemented therein constitute        **
** confidential information and may compromise trade secrets of SigmaTel,    **
** Inc. or its associates, and any unauthorized use thereof is prohibited.   **
**                                                                           **
******************************************************************************/

#ifndef __REFERENCE_H__
#define __REFERENCE_H__

#undef  EXTERN 
#ifdef REF_ALLOC
#define EXTERN
#else
#define EXTERN  extern
#endif

#define FLAG_SCAN_START        (1 << 0)
#define FLAG_TX_START          (1 << 1)
#define FLAG_RX_START          (1 << 2)
#define FLAG_PRINT_START       (1 << 3)
#define FLAG_SCAN_COMPLETE     (1 << 4)
#define FLAG_TX_COMPLETE       (1 << 5)
#define FLAG_RX_COMPLETE       (1 << 6)
#define FLAG_PRINT_COMPLETE    (1 << 7)
#define FLAG_ERORR_DETECT      (1 << 8)
#define FLAG_SCAN_CONTINUE     (1 << 9)
#define FLAG_RX_CONTINUE       (1 << 10)
#define FLAG_TX_CONTINUE       (1 << 11)
#define FLAG_PRINT_CONTINUE    (1 << 12)
#define FLAG_SCAN_ABORT		   (1 << 13)
#define FLAG_TX_ABORT		   (1 << 14)
#define FLAG_TX_ERROR		   (1 << 15)
#define FLAG_RX_ABORT		   (1 << 16)
#define FLAG_RX_ERROR		   (1 << 17)
#define FLAG_PRINT_ABORT 	   (1 << 18)
#define FLAG_PRINT_ERROR 	   (1 << 19)
#define FLAG_SCAN_ABORT_DONE 	   (1 << 20)
#endif // #ifndef __REFERENCE_H__
