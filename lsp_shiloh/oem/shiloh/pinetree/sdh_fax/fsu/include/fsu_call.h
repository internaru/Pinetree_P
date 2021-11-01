/* ************************************************************************* **
**                                                                           **
**                 Proprietary and Confidential Information                  **
**                                                                           **
**                Copyright (c) 2005 Oasis Semiconductor Inc.                **
**                                                                           **
**  This software contains proprietary and confidential material which is    **
**  property of Oasis Semiconductor Inc.                                     **
**                                                                           **
** ************************************************************************* */
#ifndef __FSU_CALL_H__
  #define __FSU_CALL_H__

/******************************************************************************
*  FILE FORMAT:
*      1. ENUMERATIONS
*      2. DEFINES
*      3. STRUCTURES
*      4. ALLOCS
*      5. FUNCTIONS
******************************************************************************/
  #include "dc_fwcxdxx_lnx_api.h"
  #include "dc_type.h"

/******************************************************************************
*  1. ENUMERATIONS
******************************************************************************/
  enum
  {
    FAX_DATA_FORMAT_MH,             // Modified Huffman (T.4 1D)
    FAX_DATA_FORMAT_MR,             // Modified Read (T.4 2D)
    FAX_DATA_FORMAT_MMR,            // Modified Modified Read (T.6 2D)
    FAX_DATA_FORMAT_SIMPLE,         // JE/JD SIMPLE
    FAX_DATA_FORMAT_JBIG,           // JBIG encoded bitonal
    FAX_DATA_FORMAT_JPEG            // Color or Gray-scale
  };

	enum
	{
		FSU_ABORT_CODE_NONE,
//		FSU_ABORT_CODE_SCANNER, 
		FSU_ABORT_CODE_SCAN,
		FSU_ABORT_CODE_USER,
		FSU_ABORT_CODE_PRINT,
		FSU_ABORT_CODE_NO_MEMORY,
		FSU_ABORT_CODE_DOC_TOO_BIG,
//		FSU_ABORT_CODE_CANT_SEND = 0xE1
    FSU_ABORT_CODE_CANT_SEND,
    FSU_ABORT_ENCODE_ERROR,
    FSU_ABORT_DECODE_ERROR
	};


/******************************************************************************
*  2. DEFINES
******************************************************************************/

  #define T30_VR_R8_X_3_85  0x00  // l/mm NORMAL (approx. 200 x 100 dpi)
  #define T30_VR_R8_X_7_7   0x01  // l/mm FINE   (approx. 200 x 200 dpi)
  #define T30_VR_R8_X_15_4  0x02  // l/mm        (approx. 200 x 400 dpi)
  #define T30_VR_R16_X_15_4 0x04  // l/mm        (approx. 400 x 400 dpi)
  #define T30_VR_200_X_100  0x08  // dpi
  #define T30_VR_200_X_200  0x10  // dpi
  #define T30_VR_400_X_400  0x20  // dpi
  #define T30_VR_300_X_300  0x40  // dpi

	#define ETX          0x03
	#define FAX_CTRL_DC2 0x12
	#define CAN          0x18
	#define FAX_CTRL_SUB 0x1A
		
	// fax function bit assignments
// fax function bit assignments
#define FAX_SCAN_TO_MEM				(1 << 0)
#define FAX_MEM_TO_SEND				(1 << 1)		// mem_to_send with fipp
#define FAX_RECEIVE_TO_MEM			(1 << 2)		// receive_to_mem with fipp
#define FAX_MEM_TO_PRINT			(1 << 3)
#define FAX_MEM_TO_SEND_BEGIN		(1 << 4)
#define FAX_RECEIVE_TO_MEM_BEGIN	(1 << 5)
#define FAX_MEM_TO_SEND_DIALING		(1 << 6)		// add 11-29



/******************************************************************************
*  3. STRUCTURES
******************************************************************************/


/******************************************************************************
*  4. ALLOCS
******************************************************************************/


/******************************************************************************
*  5. FUNCTIONS
******************************************************************************/
void fsu_task (void);
UINT8 on_hook_det (void);
void set_fsu_state(UINT8 state);
UINT8 get_fsu_state(void);
void init_fax_rcv_job(void);
extern UINT8 T30TxCheckErrors( void );
extern void monitor_modem_idle (void);
extern void set_modem_idle (void);

#endif  // __FSU_CALL_H__
