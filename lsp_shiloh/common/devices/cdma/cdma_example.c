/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 
ERROR_TYPE memtransfer(void *Dest, void *Src, int n)
{
    DEVICE_HANDLE handle = (DEVICE_HANDLE)-1;
    CDMA_CONFIG MyCfg;
    //
    // Check to see if we have enough bytes to use cdma.  Also we have to be 
    // aligned on 4 byte boundary for the dma to work optimally.
    //
    if((n > TRIGGER_POINT) &&
       !((UINT32)Dest & 0x3) &&
       !((UINT32)Src & 0x3) &&
       (CdmaOpen(CDMA_FIND_CHANNEL, &handle) == OK)) {  // get first available channel

        MyCfg.SrcBurst = CDMA_BURST_8;                  // maximum burst size for memory transfer
        MyCfg.DestBurst = CDMA_BURST_8;
        MyCfg.SrcXferWidth = CDMA_32_BITS;              // 32-bit word size 
        MyCfg.DestXferWidth = CDMA_32_BITS;
        MyCfg.SourceEndian = CDMA_ABCD_TO_ABCD;         // endian with no reorder
        MyCfg.DestEndian = CDMA_ABCD_TO_ABCD;
        MyCfg.FlowControl = CDMA_MEM_TO_MEM;            // memory to memory
        MyCfg.SrcIncrement = CDMA_INC;                  // increment addresses
        MyCfg.DestIncrement = CDMA_INC;
        MyCfg.DestModify = CDMA_NO_MOD_DEST;            // transfer data from source address
        MyCfg.TimeBase = CDMA_1_US;                     // NEED A COMMENT HERE TO EXPLAIN THIS!!!
        CdmaSetConfig(handle, &MyCfg);                  // configure channel

        if( n != CdmaTransfer(handle, Src, Dest, n, 1000, NULL, NULL)) {    // do blocking transfer with 1000 tick timeout
            ASSERT(NULL == &"memtransfer: INCOMPLETE CDMA transfer");
            memcpy(Dest,Src,n);
        }

        CdmaClose(handle);                              // close channel
    } else {
        //
        // This is either a short xfer or not aligned or no channel available, so just do it with memcpy.
        //
        memcpy(Dest,Src,n);
    }
    return OK;
}