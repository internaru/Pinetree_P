/*
 *  $RCSfile: Config.h,v $
 * $Revision: 1.6 $
 *   $Author: miurat $
 *     $Date: 2010/04/30 23:57:57 $
 */
/************************************************************************
 *
 *      CONFIG.H
 *
 *  System Configuration
 *
 *  Copyright 2004 - 2009 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif
#define Disabled        0
#define Enabled         1
#define Installed       1
#define External        1
#define Internal        2
#define ENDIAN_BIG      0
#define ENDIAN_LITTLE   1

/* Endian Selection */
#define ENDIAN ENDIAN_LITTLE

#ifndef BUFF_PRINT
#define BUFF_PRINT  Disabled    /* Buffered Printing     */
#endif              /* (Enabled/Disabled) */

/*   SpeakerPhone  */
#ifndef SPKRPHN
#define SPKRPHN     Disabled    /* Speakerphone */
#endif              /* (Enabled/Disabled)     */

/*   Telephone Answering Machine */
#ifndef TAM
#define TAM         Disabled    /* Digital answering machine */
#endif              /* (Internal/External/Disabled) */
