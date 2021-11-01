
/********************************************************************** 
 * (c) Copyright 2004 Agilent Technologies, Inc.
 *
 * 
===================================================================
 * AGILENT TECHNOLOGIES, INC. / Imaging Electronics Division (IED) 
 *
 * AGILENT INTELLECTUAL PROPERTY 
 * Agilent Intellectual Property (IP) provides significant value
 * to customer applications. Agilent IED desires to attract potential 
 * customers by enabling them to create differentiated end-products 
 * by using its value-adding IP. Given that, Agilent IED requests potential 
 * customers to contact IED Marketing Department's Patrick McKinley, 
 * (541) 738-3494, for specific information relating to design
 * access and use.
 * 
===================================================================
 * NOTICE TO USERS - LICENSE REQUIRED - AGILENT TECHNOLOGIES, INC. 
 * PROPRIETARY AND CONFIDENTIAL
 *
 * The information contained herein embodies intellectual property that 
 * belongs to Agilent Technologies, Inc., and in some cases to other 
 * companies as well. NO LICENSE IS GRANTED by the mere distribution 
 * or possession of this information.
 *
 * USAGE RESTRICTIONS. The HDL and information contained herein may 
 * not be used unless a written license to use has been granted by Agilent IED. 
 * Possession of this information does not grant permission to the user to decompile,
 * disassemble, redistribute, or reverse engineer any portion of the HDL or other
 * code, including derivative works. In the event that a license is granted to do so,
 * a license fee or royalty may be charged for such a license.
 *
 * MANUFACTURING RESTRICTIONS. The recipients of this information may 
 * not manufacture integrated circuits containing derivations of this HDL unless the 
 * written license to manufacture has been granted by Agilent Technologies, Inc.. 
 * A license fee or royalty may be charged for such a license.
 *
************************************************************************ 

File: 
    JbigHW.c

Description: 
    functions that interface with the JBIG encoder/decoder hardware 

History: 
    2004-Aug-3  JK  Initial version.

***************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>            // defines isprint()

#include "agConnectMgr.h"     // defines ParserRegister(), etc.
#include "agMessage.h"        // defines MESSAGE struct

#include "lassert.h"           // defines _assert()
#include "dprintf.h"          // defines dprintf()

#include "logger.h"        // defines ALogString()

////////////////////////////////////////////////////////////////////////////////
// Defines and Typedefs
////////////////////////////////////////////////////////////////////////////////

/* define VERBOSE_DEBUG for blow-by-blow account of activity */
//#define VERBOSE_DEBUG
//#if defined(VERBOSE_DEBUG) && !defined(NDEBUG)
//# define DPRINTF(x) dprintf x
//#else
//# define DPRINTF(x) ((void)0)
//#endif


#define LOGEVENT ALogString
#define JBCTRL_ATMOV_8   0x1000;
#define JBCTRL_ATMOV_16  0x10000;



////////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Statics (local globals)
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//  JBIGConvert() - Encode - Decode
//
////////////////////////////////////////////////////////////////////////////////

bool JBIGConvert( bool  bEncodeNotDecode, const uint8_t * pjSourceBuf, uint32_t nSourceBytes,
                  uint8_t * pjTargetBuf, uint32_t uiTargetBufSize, uint32_t * pnTargetBytes,
                  uint32_t nRows, uint32_t nCols, uint32_t nPadRows , bool Rotate_Bottom_to_Top)
{

    return true;
} // JBIGConvert()




////////////////////////////////////////////////////////////////////////////////
//  JBIGConvertDec  - Jbig Decode only
//
////////////////////////////////////////////////////////////////////////////////
#if 0
bool JBIGConvertDec( bool  bEncodeNotDecode, const uint8_t * pjSourceBuf, UINT nSourceBytes,
                  uint8_t * pjTargetBuf, UINT uiTargetBufSize, UINT * pnTargetBytes,
                  UINT nRows, UINT nCols, UINT nPadRows )
{

    return 0;
} // JBIGConvert()
#endif

