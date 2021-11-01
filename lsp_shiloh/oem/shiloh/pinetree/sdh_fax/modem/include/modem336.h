/*
 *  $RCSfile: modem336.h,v $
 * $Revision: 1.22 $
 *   $Author: miurat $
 *     $Date: 2012/08/21 22:15:57 $
 */
/************************************************************************
 *
 *      modem336.h
 *
 *  Modem device driver.
 *
 *  Copyright 2004 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

/*
Modem Host Interface Memory Map

---------------------------------------------------------------------------
   Modem Host Interface (Scratch Pad) Memory Map                          |
---------------------------------------------------------------------------
Register |            Bit Number                                          |
 Offset  |----------------------------------------------------------------|
(Default)|   7   |   6   |   5   |    4   |   3   |   2   |   1   |   0   |
---------|----------------------------------------------------------------|
 1F (00) | NSIA  | NCIA  |       | NSIE   | NEWS  | NCIE  |       | NEWC  |
---------|----------------------------------------------------------------|
 1E (00) | TDBIA | RDBIA | TDBIE |        | TDBE  | RDBIE |       | RDBF  |
---------|----------------------------------------------------------------|
 1D (00) | MEACC |MEACC1 | MEMW  | MEMCR  |           MEADDH              |
---------|----------------------------------------------------------------|
 1C (00) |                           MEADDL                               |
---------|----------------------------------------------------------------|
 1B (00) | EDET  | DTDET |  OTS  | DTMFD  |            DTMFW              |
---------|----------------------------------------------------------------|
 1A (00) | SFRES | RIEN  | RION  |        |                               |
---------|----------------------------------------------------------------|
 19 (00) |                           MEDAM                                |
---------|----------------------------------------------------------------|
 18 (00) |                           MEDAL                                |
---------|----------------------------------------------------------------|
 17 (00) |                         TXHSKSTAT                              |
---------|----------------------------------------------------------------|
 16 (00) |                         RXHSKSTAT                              |
---------|----------------------------------------------------------------|
 15 (00) | SLEEP | STOP  | RDWK  | HWRWK  |       |       | EXL3  | EARC  |
---------|----------------------------------------------------------------|
 14 (84) |                           ABCODE                               |
---------|----------------------------------------------------------------|
 13 (84) |           TLVL                 |      RTH      |    TXCLK      |
---------|----------------------------------------------------------------|
 12 (84) |                           CONF                                 |
---------|----------------------------------------------------------------|
 11 (84) | BRKS  |       |       |  TXV   |  RXV  | HDXEn | TEOF  |       |
---------|----------------------------------------------------------------|
 10 (84) |                           TBUFFER                              |
---------|----------------------------------------------------------------|

---------------------------------------------------------------------------
   Modem Host Interface (Scratch Pad) Memory Map                          |
---------------------------------------------------------------------------
Register |            Bit Number                                          |
 Offset  |----------------------------------------------------------------|
(Default)|   7   |   6   |   5   |    4   |   3   |   2   |   1   |   0   |
---------|----------------------------------------------------------------|
 0F (00) | RLSD  |  FED  |  CTS  |   DSR  |  RI   |       |       |       |
---------|----------------------------------------------------------------|
 0E (00) |       | BRKD  |       |                  SPEED                 |
---------|----------------------------------------------------------------|
 0D (00) | P2DET | PNDET |       |        |       | PCOFF | TXFNF |       |
---------|----------------------------------------------------------------|
 0C (00) | SDNS  | SDBS  | SDMA  | CCMODE | SDET  | SNDET | RXFNE |       |
---------|----------------------------------------------------------------|
 0B (00) | TONEA | TONEB | TONEC | ATV25  |       | FLAGDL| PJAT  | EQMAT |
---------|----------------------------------------------------------------|
 0A (00) | PNSUC | FLAGDT|  PE   |  FE    |  OE   | CRCS  | FLAGS | SYNCD |
---------|----------------------------------------------------------------|
 09 (00) |  NV25 |  CC   |  DTMF |  ORG   |       | DATA  |       |  DTR  |
---------|----------------------------------------------------------------|
 08 (00) |  ASYN | TPDM  | V21S  |        |       |       | RTRN  |  RTS  |
---------|----------------------------------------------------------------|
 07 (00) |       |       |       |        | L3ACT |       |  RA   | MHLD  |
---------|----------------------------------------------------------------|
 06 (00) | RTDIS |       | CCRTN |  HDLC  |       |       | DECBITS/WDSZ  |
---------|----------------------------------------------------------------|
 05 (00) | FCCR  | CEQS  | VINIT |  TXSQ  |  CEQ  | TTDIS | STOFF |       |
---------|----------------------------------------------------------------|
 04 (84) |       | EQT2  |       | FIFOEN | EQFZ  | NRZIEN|  TOD  | STRN  |
---------|----------------------------------------------------------------|
 03 (84) |  EPT  | SEPT  |       |        |       |       |       |       |
---------|----------------------------------------------------------------|
 02 (84) |  TDE  | SQDIS | DCDEN |  CDEN  |       |       |    CODBITS    |
---------|----------------------------------------------------------------|
 01 (84) |    HSVOLUME   |  PFR  |        |       | THXF  | RXHF  |       |
---------|----------------------------------------------------------------|
 00 (84) |                          RBUFFER                               |
---------|----------------------------------------------------------------|
*/

/*  Modem Register and Bit Definitions */

#include "MACROS.H"

#define DBG_MODEM_PRINTF
#define DBG_HDLC_PRINTF
//#define DBG_IRQ_PRINTF
//#define DBG_DSP_PRINTF
//#define DBG_SDAA_PRINTF

#ifdef CX_DEBUG

#if 0
// The following defines were moved to cxshell.h
#define TRACE_HDLCDEBUG   if(_CxDebugLevel==(DEBUG_HDLC|DEBUG_DEBUG)) CxDebugPrint
#define TRACE_MODEMDEBUG  if(_CxDebugLevel==(DEBUG_MODEM|DEBUG_DEBUG)) CxDebugPrint
#define TRACE_SDAADEBUG   if(_CxDebugLevel==(DEBUG_SDAA|DEBUG_DEBUG)) CxDebugPrint
#define TRACE_IRQDEBUG    if(_CxDebugLevel==(DEBUG_IRQ|DEBUG_DEBUG)) CxDebugPrint

#define TRACE_CONTINUE_HDLC  if(_CxDebugLevel==(DEBUG_HDLC|DEBUG_SHELL)) CxDebugPrint
#define TRACE_CONTINUE_MODEM if(_CxDebugLevel==(DEBUG_MODEM|DEBUG_SHELL)) CxDebugPrint
#define TRACE_CONTINUE_SDAA  if(_CxDebugLevel==(DEBUG_SDAA|DEBUG_SHELL)) CxDebugPrint
#define TRACE_CONTINUE_IRQ   if(_CxDebugLevel==(DEBUG_IRQ|DEBUG_SHELL)) CxDebugPrint
#endif

#else
#define TRACE_HDLCDEBUG
#define TRACE_MODEMDEBUG
#define TRACE_SDAADEBUG
#define TRACE_IRQDEBUG
#define TRACE_CONTINUE_HDLC
#define TRACE_CONTINUE_MODEM
#define TRACE_CONTINUE_SDAA
#define TRACE_CONTINUE_IRQ
#endif

#define SPI_MULTI_DATA

#define MALIGN
//#define MALIGN(a) (a << 1)

#define MODEM_1F        MALIGN(0x1F)    /* SPX register 0x1F */
#define NSIA            0x80            /* New status interrupt active */
#define NCIA            0x40            /* New configuration interrupt active */
#define NSIE            0x10            /* New status interrupt enable */
#define NEWS            0x08            /* New status */
#define NCIE            0x04            /* New configuration interrupt enable */
#define NEWC            0x01            /* New configuration */

#define MODEM_1E        MALIGN(0x1E)    /* SPX register 0x1F */
#define TDBIA           0x80            /* Transmit data buffer interrupt active */
#define RDBIA           0x40            /* Receive data buffer interrupt active */
#define TDBIE           0x20            /* Transmit data buffer interrupt enable */
#define TDBE            0x08            /* Transmit data buffer empty */
#define RDBIE           0x04            /* Receive data buffer interrupt enable */
#define RDBF            0x01            /* Receive data buffer full */

#define MODEM_1D        MALIGN(0x1D)    /* SPX Register 0x1D */
#define MODEM_MEADDH    MODEM_1D        /* Memory Access Register High */
#define MEACC           0x80            /* Memory Access Enable  */
#define MEACC1          0x40            /* Memory Access Enable locations 0xC00-0xC5F */
#define MEMW            0x20            /* Memory Write  */
#define MEMCR           0x10            /* Memory Continuous Read */

#define MODEM_1C        MALIGN(0x1C)    /* SPX Register 0x1C */
#define MODEM_MEADDL    MODEM_1C        /* Memory Access Register Low */

#define MODEM_1B        MALIGN(0x1B)    /* SPX Register 0x1B */
#define MODEM_DTMFW     MODEM_1B        /* DTMF Output Word */
#define EDET            0x80            /* DTMF Early Detection  */
#define DTDET           0x40            /* Dual Tone Detected  */
#define OTS             0x20            /* DTMF On Time Satisfied */
#define DTMFD           0x10            /* DTMF Signal Detected */

#define MODEM_1A        MALIGN(0x1A)    /* SPX Register 0x1A */
#define SFRES           0x80            /* Soft Reset  */
#define RIEN            0x40            /* RION Enabled  */
#define RION            0x20            /* Ring Indicator On */

#define MODEM_19        MALIGN(0x19)    /* SPX Register 0x19 */
#define MODEM_MEDAM     MODEM_19        /* Memory Data MSB */

#define MODEM_18        MALIGN(0x18)    /* SPX Register 0x18 */
#define MODEM_MEDAL     MODEM_18        /* Memory Data LSB */

#define MODEM_17        MALIGN(0x17)    /* SPX Register 0x17 */
#define MODEM_TXHSKSTAT MODEM_17        /* V.34 Transmit Status */
#define CC_DATA_MODE    0x84
#define CC_INIT         0x80
#define PC_INIT         0xA0
#define PC_TX_DATA_MODE 0xA7
#define PC_RX_DATA_MODE 0xA3

#define MODEM_16        MALIGN(0x16)    /* SPX Register 0x16 */
#define MODEM_RXHSKSTAT MODEM_16        /* V.34 Receive Status */

#define MODEM_15        MALIGN(0x15)    /* SPX Register 0x15 */
#define MSLEEP          0x80            /* Sleep Mode  */
#define MSTOP           0x40            /* Stop Mode  */
#define RDWK            0x20            /* Ring Detect Wake Up  */
#define HWRWK           0x10            /* Host Write Wake Up */
#define EXL3            0x02            /* External Loop 3 Selector  */
#define EARC            0x01            /* Extended Automatic Range Change */

#define MODEM_14        MALIGN(0x14)    /* SPX Register 0x14 */
#define MODEM_ABCODE    MODEM_14        /* Abort Code */

#define SpDTMF          0x80
#define TonSpkE         0x40
#define TonLinE         0x20

#define MODEM_13        MALIGN(0x13)    /* SPX Register 0x13 */
#define MODEM_TLVL      MODEM_13        /* Transmit Level */
                                        /* -0dB (0x00) - -15dB (0xF0) */
#define MODEM_RTH       MODEM_13        /* Receiver Threshold */
#define RTH_ON43_OFF48  0x00
#define RTH_ON33_OFF38  0x04
#define RTH_ON26_OFF31  0x08
#define RTH_ON16_OFF21  0x0C

#define MODEM_TXCLK     MODEM_13        /* Transmit Clock Select */
#define TXCLK_INTERNAL  0x00
#define TXCLK_EXTERNAL  0x02
#define TXCLK_SLAVE     0x03

#define MUTESp          0x08
#define MUTEMic         0x04

#define MODEM_MICLVL    MODEM_13        /* Handset Mic Level */
#define MICLVL_HIGH     0x03            /* +15.6dB */
#define MICLVL_MEDIUM   0x02            /* + 9.6dB */
#define MICLVL_LOW      0x01            /* + 5.6dB */
#define MICLVL_MINIMUM  0x00            /* 0dB */

#define MODEM_12                    MALIGN(0x12)    /* SPX Register 0x12 */
#define MODEM_CONF                  MODEM_12        /* Configuration */
#define MODEM_V8_CONF               0xAA
#define MODEM_V34_33600_CONF        0xCE
#define MODEM_V34_31200_CONF        0xCD
#define MODEM_V34_28800_CONF        0xCC
#define MODEM_V34_26400_CONF        0xCB
#define MODEM_V34_24000_CONF        0xCA
#define MODEM_V34_21600_CONF        0xC9
#define MODEM_V34_19200_CONF        0xC8
#define MODEM_V34_16800_CONF        0xC7
#define MODEM_V34_14400_CONF        0xC6
#define MODEM_V34_12000_CONF        0xC5
#define MODEM_V34_9600_CONF         0xC4
#define MODEM_V34_7200_CONF         0xC3
#define MODEM_V34_4800_CONF         0xC2
#define MODEM_V34_2400_CONF         0xC1
#define MODEM_V33_14400_CONF        0x31
#define MODEM_V33_12000_CONF        0x32
#define MODEM_V33_9600_CONF         0x34
#define MODEM_V33_7200_CONF         0x38
#define MODEM_V17_14400_CONF        0xB1
#define MODEM_V17_12000_CONF        0xB2
#define MODEM_V17_9600_CONF         0xB4
#define MODEM_V17_7200_CONF         0xB8
#define MODEM_V29_9600_CONF         0x14
#define MODEM_V29_7200_CONF         0x12
#define MODEM_V29_4800_CONF         0x11
#define MODEM_V27_4800_CONF         0x02
#define MODEM_V27_2400_CONF         0x01
#define MODEM_V23_1200TX_75RX_CONF  0xA4
#define MODEM_V23_75TX_1200RX_CONF  0xA1
#define MODEM_V21_CONF              0xA0
#define MODEM_V21_CH2_CONF          0xA8
#define MODEM_SINGLE_TONE_CONF      0x80
#define MODEM_DUAL_TONE_CONF        0x83
#define MODEM_DIALING_CONF          0x81
#define MODEM_DTMF_RX_CONF          0x86
#define MODEM_ADPCM_CONF            0xAC

#define MODEM_11  MALIGN(0x11)      /* SPX Register 0x11 */
#define BRKS            0x80        /* Send Break Sequence */
#define TXV             0x10        /* Transmit Voice */
#define RXV             0x08        /* Receive Voice */
#define HDXEN           0x04        /* V.23 Half Duplex Enable */
#define TEOF            0x02        /* HDLC Transmit End of Frame */

#define MODEM_PARSL MODEM_11        /* Parity Select */
#define PARSL_STUFF     0x00
#define PARSL_SPACE     0x20
#define PARSL_EVEN      0x40
#define PARSL_ODD       0x60

#define MODEM_10  MALIGN(0x10)      /* SPX Register 0x10 */
#define MODEM_TBUFF     MODEM_10    /* Transmit Buffer */

#define MODEM_0F  MALIGN(0x0F)      /* SPX Register 0x0F */
#define RLSD            0x80        /* Received Line Signal Detector */
#define FED             0x40        /* Fast Energy Detector */
#define CTS             0x20        /* Clear To Send */
#define DSR             0x10        /* Data Set Ready */
#define RI              0x08        /* Ring Indicator */

#define MODEM_0E  MALIGN(0x0E)      /* SPX Register 0x0E */
#define BRKD            0x40        /* Break Detected */

#define MODEM_SPEED_REG     MODEM_0E    /* Speed Indication */
#define MODEM_SPEED_300     0x00
#define MODEM_SPEED_600     0x01
#define MODEM_SPEED_1200    0x02
#define MODEM_SPEED_2400    0x03
#define MODEM_SPEED_4800    0x04
#define MODEM_SPEED_9600    0x05
#define MODEM_SPEED_12000   0x06
#define MODEM_SPEED_14400   0x07
#define MODEM_SPEED_7200    0x08
#define MODEM_SPEED_16800   0x09
#define MODEM_SPEED_19200   0x0A
#define MODEM_SPEED_21600   0x0B
#define MODEM_SPEED_24000   0x0C
#define MODEM_SPEED_26400   0x0D
#define MODEM_SPEED_28800   0x0E
#define MODEM_SPEED_31200   0x0F
#define MODEM_SPEED_33600   0x10

#define MODEM_0D  MALIGN(0x0D)      /* SPX Register 0x0D */
#define P2DET           0x80        /* P2 Sequence Detected */
#define PNDET           0x40        /* PN Sequence Detected */
#define PCOFF           0x04        /* Primary Channel to Control Channel Transition */
#define TXFNF           0x02        /* Transmitter FIFO not Full */

#define MODEM_0C  MALIGN(0x0C)      /* SPX Register 0x0C */
#define SDNS            0x80        /* SDAA New Status */
#define SDBS            0x40        /* SDAA Busy */
#define SDMA            0x20        /* SDAA Monitor Active */
#define CCMODE          0x10        /* Control Channel Mode */
#define TPMT            0x10        /* Transmitter Pipe Empty */
#define SDET            0x08        /* S Detector */
#define TXEFIFOsi       0x08        /* Transmit EFIFO Below Threshold */
#define SNDET           0x04        /* S Negative Detector */
#define RXEFIFOsi       0x04        /* Receive EFIFO Below Threshold */
#define RXFNE           0x02        /* Receiver FIFO not empty */

#define MODEM_0B  MALIGN(0x0B)      /* SPX Register 0x0B */
#define TONEA           0x80        /* Tone A Detected */
#define TONEB           0x40        /* Tone B Detected */
#define TONEC           0x20        /* Tone C Detected */
#define ATV25           0x10        /* V.25 Answer Tone Detected */
#define FLAFDTL         0x04        /* V.21 Channel 1 Flag Detected */
#define PJAT            0x02        /* Phase Jitter Above Threshold */
#define EQMAT           0x01        /* EQM Above Threshhold */

#define MODEM_0A  MALIGN(0x0A)      /* SPX Register 0x0A */
#define PNSUC           0x80        /* PN Success */
#define FLAGDT          0x40        /* V.21 Channel 2 Flag Detected */
#define PE              0x20        /* Parity Error */
#define FE              0x10        /* Framing Error */
#define OE              0x08        /* Overrun Error */
#define CRCS            0x04        /* CRC Sending */
#define FLAGS           0x02        /* Flag Sequence */
#define SYNCD           0x01        /* Sync Pattern Detected */

#define MODEM_09  MALIGN(0x09)      /* SPX Register 0x09 */
#define NV25            0x80        /* No V.25 Answer Tone */
#define CC              0x40        /* Controlled Carrier */
#define DTMF            0x20        /* DTMF Select */
#define ORG             0x10        /* Originate */
#define DATA            0x04        /* Data */
#define DTR             0x01        /* Data Terminal Ready */

#define MODEM_08  MALIGN(0x08)      /* SPX Register 0x08 */
#define ASYN            0x80        /* Asynchronous Mode */
#define TPDM            0x40        /* Transmitter Parallel Data Mode */
#define V21S            0x20        /* V.21 Synchronous */
#define RTRN            0x02        /* Retrain */
#define RTS             0x01        /* Request To Send */

#define MODEM_07  MALIGN(0x07)      /* SPX Register 0x07 */
#define L3ACT           0x08        /* Loop 3 Activate */
#define RA              0x02        /* Relay A Active */
#define MHLD            0x01        /* Mark Hold */

#define MODEM_06  MALIGN(0x06)      /* SPX Register 0x06 */
#define RTDIS           0x80        /* Receiver Training Disable */
#define CCRTN           0x20        /* Control Channel Retrain */
#define HDLC            0x10        /* HDLC Select */
#define WDSZ_8          0x03        /* Data Word Size: 8-bit */

#define MODEM_05  MALIGN(0x05)      /* SPX Register 0x05 */
#define FCCR            0x80        /* Force Control Channel Mode in Recipient Modem */
#define CEQS            0x40        /* Compromise Equalizer Select */
#define VINIT           0x20        /* Voice Initiate */
#define TXSQ            0x10        /* Transmitter Squelch */
#define CEQ             0x08        /* Compromise Equalizer Enable */
#define TTDIS           0x04        /* Transmitter Training disable */
#define STOFF           0x02        /* Soft Turn Off */

#define MODEM_04  MALIGN(0x04)      /* SPX Register 0x04 */
#define EQT2            0x40        /* Equalizer T/2 Spacing Select */
#define FIFOEN          0x10        /* FIFO Enable */
#define EQFZ            0x08        /* Equalizer Freeze */
#define NRZIEN          0x04        /* NRZI Enable */
#define TOD             0x02        /* Train on Data */
#define STRN            0x01        /* Short Train Select */

#define MODEM_03  MALIGN(0x03)      /* SPX Register 0x03 */
#define EPT             0x80        /* Echo Protector Tone Enable */
#define SEPT            0x40        /* Short Echo Protector Tone */

#define MODEM_02  MALIGN(0x02)      /* SPX Register 0x02 */
#define TDE             0x80        /* Tone Detectors Enable */
#define SQDIS           0x40        /* Squarer Disable (Tone Detector C) */
#define DCDEN           0x20        /* Decoder Enable */
#define CDEN            0x10        /* Coder Enable */
#define CODBITS         0x03        /* Coder Number of Bits */

#define MODEM_01  MALIGN(0x01)      /* SPX Register 0x01 */
#define PFR             0x20        /* Preload FIFO Ready */
#define TXHF            0x04        /* Transmitter FIFO half full */
#define RXHF            0x02        /* Receiver FIFO half full */

#define MODEM_VOLUME    MODEM_01    /* Handset Speaker Volume */
#define VOLUME_HIGH     0xC0        /* +7dB */
#define VOLUME_MEDIUM   0x80        /* +1dB */
#define VOLUME_LOW      0x40        /* -4dB */
#define VOLUME_OFF      0x00        /* Mute */

#define MODEM_00        MALIGN(0x00)    /* SPX Register 0x00 */
#define MODEM_RBUFF     MODEM_00        /* Receive Buffer */


// General Control/Status Register
#define CTRL_REG0       MALIGN(0x2E)
#define SRESET_MODEM    0x04
#define CTRL_REG1       MALIGN(0x2D)
#define IRQ_IMSR        MALIGN(0x2A)
#define IRQ_IER         MALIGN(0x29)
#define ENABLE_SSDIRQ   0x80            /* enable SSD interrupt */

#define BURST_COUNT_REG MALIGN(0x27)
#define GPIO_DIR        MALIGN(0x26)
#define ENABLE_GPO3     0x08

#define GPO_DATA        MALIGN(0x25)
#define GPIO_3          0x08

#define GPI_DATA        MALIGN(0x24)
#define GPO_DRV1        MALIGN(0x23)
#define GPO_DRV0        MALIGN(0x22)
#define IRQ_POL         MALIGN(0x21)
#define IRQ_TYPE        MALIGN(0x20)

/* Modem DSP RAM Access Codes */
#define MODEM_BOND_OPTION_CHECK     0x74F       /* Set bit6, disable to ckeck 2nd IA  */

/* Interrupt Request Handling */
#define MODEM_NEWS_MASK_1B          0x27C       /* New Status MAsking Register 1B */
#define MODEM_NEWS_MASK_1A          0x27D       /* New Status MAsking Register 1A */
#define MODEM_NEWS_MASK_17          0x371       /* New Status Masking Register 17 */
#define MODEM_NEWS_MASK_16          0x370       /* New Status Masking Register 16 */
#define MODEM_NEWS_MASK_14          0x38A       /* New Status Masking Register 14 */
#define MODEM_NEWS_MASK_12          0x089       /* New Status Masking Register 12 (bit 7) */
#define MODEM_NEWS_MASK_0F          0x241       /* New Status Masking Register 0F */
#define MODEM_NEWS_MASK_0E          0x242       /* New Status Masking Register 12 */
#define MODEM_NEWS_MASK_0D          0x243       /* New Status Masking Register 12 */
#define MODEM_NEWS_MASK_0C          0x244       /* New Status Masking Register 12 */
#define MODEM_NEWS_MASK_0B          0x245       /* New Status Masking Register 12 */
#define MODEM_NEWS_MASK_0A          0x246       /* New Status Masking Register 12 */
#define MODEM_NEWS_MASK_01          0x247       /* New Status Masking Register 01 */
#define MODEM_NEWS_MASK_MEM_ACCESS  0x089       /* New Status Masking Register Mem Access (bit 6) */
#define MODEM_NEWS_TX_EOF           0x02
#define MODEM_NEWS_RX_EOF           0x39
#define MODEM_MEM_ACCESS_DISABLE    0x40

/* FIFO */
#define MODEM_RX_FIFO               0x32C       /* Receiver FIFO */
#define MODEM_RX_FIFO_TRG_LVL_MASK  0xC0        /* Triggel Level Mask */
#define MODEM_RX_FIFO_TOE_MASK      0x01        /* Time out Enable Mask */
#define MODEM_RX_FIFO_TOS_MASK      0x1C        /* Time out Select Mask */
#define MODEM_RX_FIFO_CLEAR         0x031       /* RXFIFO Clear (bit 3) */
#define MODEM_TX_FIFO_CLEAR         0x031       /* TXFIFO Clear (bit 7) */
#define MODEM_TX_EXTEND_FIFO_DIS    0x0E8       /* Transmit Extend FIFO Disable (bit 0) */
#define MODEM_RX_EXTEND_FIFO_DIS    0x0E8       /* Receive Extend FIFO Disable (bit 1) */
#define MODEM_NEWS_INT_MODE_ENABLE  0x0E8       /* NEWS interrupt modem enable (bit 4) */
#define MODEM_REINIT_EXTEND_FIFO    0x0E8       /* Reinitialize Extend FIFO (bit 7) */
#define MODEM_TX_FIFO_EMPTY_THRESH  0x685       /* Transmitter FIFO empty threshhold */
#define MODEM_RX_FIFO_FULL_THRESH   0x686       /* Receiver FIFO buffer full threshhold */
#define MODEM_TX_FIFO_IN_POINTER    0x70A       /* Transmitter FIFO in pointer */
#define MODEM_TX_FIFO_OUT_POINTER   0x70B       /* Transmitter FIFO out pointer */
#define MODEM_TX_FIFO_PRELOAD       0x6DB       /* Transmitter FIFO preload enable */

/* V.8 BIS detectors (supplemental tone detectors) */
#define MODEM_NUM_TAPS              0xB47       /* Number of Taps */
#define MODEM_TX_COMP_EQZ_COEFF     0xAD0       /* TX Compromise Eqyalizer Coefficients (AD0 - AED) */
#define MODEM_CUSTOM_CEQ_TAPS_PROT  0x6CC       /* Custom CEQ Taps Protection (bit 5) */
#define MODEM_TX_OUTPUT_GAIN_ALL_H  0x3DB       /* Tx Output level gain - all modes */
#define MODEM_TX_OUTPUT_GAIN_ALL_L  0x3DA       /* Tx Output level gain - all modes */
#define MODEM_TX_OUTPUT_GAIN_FSK    0xB57       /* Tx Output level gain - FSK modes */
#define MODEM_ANSWER_TONE_LENGTH_H  0x229       /* Answer Tone Length */
#define MODEM_ANSWER_TONE_LENGTH_L  0x228       /* Answer Tone Length */
#define MODEM_SILENCE_AFTER_TONE_H  0x22B       /* Silence after Tone Period */
#define MODEM_SILENCE_AFTER_TONE_L  0x22A       /* Silence after Tone Period */
#define MODEM_CTS_OFF_ON            0x202       /* CTS Off to On Response Time */
#define MODEM_AGC_GAIN              0xA00       /* AGC Gain Word */
#define MODEM_RLSD_ON_H             0x135       /* RLSD Turn on threshhold */
#define MODEM_RLSD_ON_L             0x134       /* RLSD Turn on threshhold */
#define MODEM_RLSD_OFF_H            0x137       /* RLSD Turn off threshhold */
#define MODEM_RLSD_OFF_L            0x136       /* RLSD Turn off threshhold */
#define MODEM_RLSD_OVERWRITE        0x10D       /* RLSD Overwrite Control (bit 2) */
#define MODEM_RLSD_THRESH_SEL       0x10D       /* RLSD Threshold selection (bit 6) */
#define MODEM_RLSD_ON_AFTER_TRAIN   0x6CC       /* RLSD Turn-On after Valid Training (bit 3) */
#define MODEM_RLSD_DROPOUT_TIMER_H  0x271       /* RLSD Dropout Timer */
#define MODEM_RLSD_DROPOUT_TIMER_L  0x270       /* RLSD Dropout Timer */
#define MODEM_RLSD_QUAL_BY_PNSUC    0x6D7       /* RLSD qualified by PNSUC (bit 3) */

/* Eye Quality Monitor */
#define MODEM_EQM                   0x20C       /* Eye Quality Monitor */
#define MODEM_EQM_THRESH            0x133       /* EQM threshhold */
#define MODEM_PHASE_JITTER_THRESH   0x6D9       /* Phase Jitter Threshold */

/* Dialing, DTMF Receiver, Tone Detection, and Tone Generation */
#define MODEM_RING_VALID_MAX        0x21F       /* Maximum Period of Valid Ring Signal */
#define MODEM_RING_VALID_MIN        0x21E       /* Minimum Period of Valid Ring Signal */
#define MODEM_DTMF_TONE_DUR_H       0x2DB       /* DTMF Tone Duration */
#define MODEM_DTMF_TONE_DUR_L       0x218       /* DTMF Tone Duration */
#define MODEM_DTMF_INTERDIG_DELAY_H 0x2DC       /* DTMF Interdigit Delay */
#define MODEM_DTMF_INTERDIG_DELAY_L 0x219       /* DTMF Interdigit Delay */
#define MODEM_PULSE_RELAY_MAKE      0x22C       /* Pulse Relay make time */
#define MODEM_PULSE_RELAY_BREAK     0x21C       /* Pulse relay break time */
#define MODEM_PULSE_INTERDIG_DELAY_H  0x21B     /* Pulse Interdigit delay */
#define MODEM_PULSE_INTERDIG_DELAY_L  0x21A     /* Pulse Interdigit delay */
#define MODEM_CALLING_TONE_ON_TIME_H  0x2D9     /* Calling Tone On Time */
#define MODEM_CALLING_TONE_ON_TIME_L  0x290     /* Calling Tone On Time */
#define MODEM_CALLING_TONE_OFF_TIME_H 0x2DA     /* Calling Tone Off Time */
#define MODEM_CALLING_TONE_OFF_TIME_L 0x291     /* Calling Tone Off Time */
#define MODEM_DTMF_LO_BAND_POWER_H  0x29C       /* DTMF Low band power level */
#define MODEM_DTMF_LO_BAND_POWER_L  0x29B       /* DTMF Low band power level */
#define MODEM_DTMF_HI_BAND_POWER_H  0x29E       /* DTMF High band power level */
#define MODEM_DTMF_HI_BAND_POWER_L  0x29D       /* DTMF High band power level */
#define MODEM_DTMF_MIN_ON_TIME      0xE96       /* DTMF minimum on time */
#define MODEM_DTMF_MIN_OFF_TIME     0xC96       /* DTMF minimum off time */
#define MODEM_DTMF_MIN_CYCLE_TIME   0xD96       /* DTMF minimum cycle time */
#define MODEM_DTMF_MIN_DROPOUT_TIME 0xF96       /* DTMF minimum dropout time */
#define MODEM_DTMF_MAX_SPEECH_EGY   0xE95       /* DTMF maximum speech energy */
#define MODEM_DTMF_FREQ_DEV_LOW     0xC94       /* DTMF frquency deviation, low group */
#define MODEM_DTMF_FREQ_DEV_HIGH    0xE94       /* DTMF frquency deviation, high group */
#define MODEM_DTMF_TWIST_NEG        0xD95       /* DTMF negative twist control */
#define MODEM_DTMF_TWIST_POS        0xC95       /* DTMF positive twist control */
#define MODEM_DTMF_MEHT             0xE87       /* DTMF maximum energy hit time */
#define MODEM_DTMF_RX_FSK_V8        0x43F       /* DTMF Receiver in FSK V.8 Modes (Bit4:FSK, Bit3:V8) */
#define MODEM_DUAL_TONE1_FREQ_H     0x281       /* Dual Tone 1 Frequency */
#define MODEM_DUAL_TONE1_FREQ_L     0x280       /* Dual Tone 1 Frequency */
#define MODEM_DUAL_TONE2_FREQ_H     0x283       /* Dual Tone 2 Frequency */
#define MODEM_DUAL_TONE2_FREQ_L     0x282       /* Dual Tone 2 Frequency */
#define MODEM_DUAL_TONE1_POWER_H    0x285       /* Dual Tone 1 Power Level */
#define MODEM_DUAL_TONE1_POWER_L    0x284       /* Dual Tone 1 Power Level */
#define MODEM_DUAL_TONE2_POWER_H    0x287       /* Dual Tone 2 Power Level */
#define MODEM_DUAL_TONE2_POWER_L    0x286       /* Dual Tone 2 Power Level */

/* HDLC Control Bits */
#define MODEM_CRC_SELECT            0x0B3       /* CRC Selection (bit 0) */
#define MODEM_CRC_16                0x01
#define MODEM_TX_ABORT_UNDERFLOW    0x3A5       /* Transmitter abort or underflow (bit 6) */
#define MODEM_TX_UNDERFLOW_BIT      0x40
#define MODEM_TX_UNDERFLOW_CODE     0x61
#define MODEM_FLAGS2TX              0x2A6       /* FLAGS2TX */


/************* V.34 HDX PHASE 1 PROCEDURES ***************/

/* V.8 Octet Monitoring (Method 1) */
#define MODEM_V8_SYNC_CM_JM_CI      0x32D       /* SYNC CM/JM/CI */
#define MODEM_V8_DATA_CALL_FXN      0x32E       /* Data Call Function */
#define MODEM_V8_MODULATION_0       0x32F       /* Modulation 0 */
#define MODEM_V8_EOF                0x32F       /* EOF */
#define MODEM_V8_MODULATION_1       0x330       /* Modulation 1 */
#define MODEM_V8_MODULATION_2       0x331       /* Modulation 2 */
#define MODEM_V8_PROTOCOL           0x332       /* Protocol (optional) */
#define MODEM_V8_GSTN               0x333       /* GSTN (optional) */
#define MODEM_V8_FRAME_END          0x334       /* Frame End */

/* V.8 Status/Control Registers (Method 1) */
#define MODEM_V8_STATUS_0           0x300       /* V.8 Status Register 0 */
#define MODEM_V8_STATUS_1           0x301       /* V.8 Status Register 1 */
#define MODEM_V8_STATUS_2           0x302       /* V.8 Status Register 2 */
#define MODEM_V8_STATUS_3           0x303       /* V.8 Status Register 3 */
#define MODEM_V8_CONTROL_1          0x304       /* V.8 Control Register 1 */
#define MODEM_V8_CONTROL_2          0x305       /* V.8 Control Register 2 */
#define MODEM_V8_CONTROL_3          0x306       /* V.8 Control Register 3 */
#define MODEM_V8_CONTROL_4          0x307       /* V.8 Control Register 4 */
#define MODEM_V8_CONTROL_5          0x308       /* V.8 Control Register 5 */
#define MODEM_V8_CONTROL_6          0x38F       /* V.8 Control Register 6 */

/* Modulation Modes */
#define MODEM_V34_HD_CONF           0x30A       /* V.34 half duplex */
#define MODEM_V17_CONF              0x30D       /* V.17 Configuration */
#define MODEM_V29_CONF              0x30E       /* V.29 Configuration */
#define MODEM_V27_CONF              0x30F       /* V.27 Configuration */
#define MODEM_V23F_CONF             0x312       /* V.23 Full Duplex Configuration */
#define MODEM_V23H_CONF             0x313       /* V.23 Half Duplex Configuration */
#define MODEM_V212_CONF             0x314       /* V.21 Channel 2 Configuration */

#define MODEM_V8_CALL_FUNCTIONS     0x32A       /* V.8 Call Functions */
#define MODEM_V8_AUTO_IPE           0x104       /* Auto Initial Polling Enable (bit 6) */
#define MODEM_V8_C1_CAD_ON_TIME     0x102       /* C1 Cadence On Time */
#define MODEM_V8_C1_CAD_OFF_TIME_H  0x3A6       /* C1 Cadence Off Time */
#define MODEM_V8_C1_CAD_OFF_TIME_L  0x3A7       /* C1 Cadence Off Time */
#define MODEM_V8_SKIP_ANSAM         0x38F       /* Skip ANSam Function (bit 3) */

/************* V.34 HDX PARAMETER SCALING ***************/

/* V.34 Symbol Rate Selection */
#define MODEM_V34_SYMBOL_RATE_SEL   0x2E3       /* Symbol rate selection */

#define MODEM_V34_BMR               0x101       /* Baud Rate Mask */
#define MODEM_V34_BMR_2400          0x03        /* 2400 Baud */
#define MODEM_V34_BMR_2800          0x06        /* 2800 Baud */
#define MODEM_V34_BMR_3000          0x0A        /* 3000 Baud */
#define MODEM_V34_BMR_3200          0x12        /* 3200 Baud */
#define MODEM_V34_BMR_3429          0x22        /* 3429 Baud */

#define MODEM_V34_GDP_DIS           0x6CC       /* Group Delay Probing Disable (bit 2) */
#define MODEM_V34_GDP_LOB           0x689       /* Group Delay Probing Low Offset Bias */
#define MODEM_V34_GDP_HOB           0x68A       /* Group Delay Probing High Offset Bias */
#define MODEM_V34_SRS_SNR_ENABLE    0x3A5       /* Symbol rate selection SNR enabled */
#define MODEM_V34_SRS_SNR_THRESH    0x6D1       /* Symbol rate selection SNR threshold */
#define MODEM_V34_SRS_FORCE_2400    0x3A5       /* Forced 2400 Symbol rate (bit 5) */
#define MODEM_V34_SPECTRAL_PARM_CTL 0x105       /* Spectral parameter control */
#define MODEM_V34_INFOH_TX_1        0x35B       /* Modem INFOh Data Transmitted (refer to page 4-23) */
#define MODEM_V34_INFOH_TX_2        0x35C       /* Modem INFOh Data Transmitted (refer to page 4-23) */
#define MODEM_V34_INFOH_TX_3        0x35D       /* Modem INFOh Data Transmitted (refer to page 4-23) */
#define MODEM_V34_INFOH_RX_1        0x365       /* Modem INFOh Data Received (refer to page 4-23) */
#define MODEM_V34_INFOH_RX_2        0x366       /* Modem INFOh Data Received (refer to page 4-23) */
#define MODEM_V34_INFOH_RX_3        0x367       /* Modem INFOh Data Received (refer to page 4-23) */

/* V.34 Transmitter Mapping, shaping, Filtering, and Precoding */
#define MODEM_V34_PHASE2_TONEA_MAX  0x6DC       /* Phase 2 Tone A Max Qualification Time */
#define MODEM_V34_TX_TRELLIS_MAP    0x375       /* Tx Trellis Mapping */
#define TRELLIS_16                  0x00
#define TRELLIS_32                  0x01
#define TRELLIS_64                  0x11
#define MODEM_V34_NON_LINEAR_ENC_TX 0xC08       /* Non-Linear Encoding Tx (bit A) */
#define MODEM_V34_NON_LINEAR_ENC_RX 0xC00       /* Non-Linear Encoding Tx (bit A) */
#define MODEM_V34_PRECODING_TX      0x2FD       /* Precoding Tx (bit 3) */
#define MODEM_V34_PRECODING_RX      0x052       /* Precoding Tx (bit 2) */
#define MODEM_V34_SHAPING_TX        0xA49       /* Precoding Tx (bit 15) */
#define MODEM_V34_SHAPING_RX        0x9F8       /* Precoding Tx (bit 15) */
#define MODEM_V34_CONSTEL_SIZE      0x38F       /* V.34 Constellation size */
#define MODEM_V34_CARRIER_FREQ_TX   0x109       /* Carrier Frequency (TX) (Bit 0) */
#define MODEM_V34_CARRIER_FREQ_RX   0x211       /* Carrier Frequency (RX) (Bit 0) */
#define MODEM_V34_PREVAL            0xB44       /* Pre-emphasis value */
#define MODEM_V34_PREOVER           0x0E6       /* Pre-emphasis override */
#define MODEM_V34_PREDIS            0x100       /* Pre-emphasis disable */

/* V.34 Power Level Functions */
#define MODEM_V34_PHASE2_POWER_RED  0x0E2       /* V.34 Phase 2 Power Reduction */
#define MODEM_V34_TLDDIS            0x100       /* Transmit level deviation disable (Bit 3) */
#define MODEM_V34_PHASE2_GUARD_TONE 0x38F       /* V.34 Phase 2 Guard Tone Level (Bit 2) */

/* V.34 Probing */
#define MODEM_V34_SIGNAL_TO_NOISE   0x11E       /* Signal to noise ratio */
#define MODEM_V34_HBAND_EDGE_FREQ   0x11D       /* High band edge frequency */
#define MODEM_V34_LBAND_EDGE_FREQ   0x11C       /* Low band edge frequency */

/* V.34 Training parameters */
#define MODEM_V34_PROG_TRAIN_DUR    0x6CF       /* Programmable training duration */
#define MODEM_V34_AUTO_TRAIN_DUR    0x6CC       /* Automatic training duration (bit 4) */
#define MODEM_V34_TRN               0x100       /* TRN 4/16 point selection (bit 7) */
#define MODEM_V34_TRN_ENABLE        0x3C1       /* TRN enable (bit 4) */
#define MODEM_V34_TRN_SNR_THRESH    0x2CD       /* TRN SNR Threshold */
#define MODEM_V34_TRN_SR_THRESH     0x6CE       /* TRN Symbol Rate Threshold */
#define MODEM_V34_TRN_SR_ENABLE     0x6CC       /* TRN Symbol Rate Enable */
#define MODEM_PHASE_JITTER_FREQ     0x80E       /* Phase Jitter Frequency */
#define MODEM_PHASE_JITTER_AMPL     0x80D       /* Phase Jitter Amplitude */
#define MODEM_PHASE3_JITTER_FREQ_H  0x6C7       /* Phase 3 Jitter Frequency */
#define MODEM_PHASE3_JITTER_FREQ_L  0x6C6       /* Phase 3 Jitter Frequency */
#define MODEM_PHASE3_JITTER_AMPL_H  0x6C9       /* Phase 3 Jitter Amplitude */
#define MODEM_PHASE3_JITTER_AMPL_L  0x6C8       /* Phase 3 Jitter Amplitude */
#define MODEM_PHASE3_RTN_THRESH4    0x699       /* Programmable Phase 3 retrain threshold 4 point */
#define MODEM_PHASE3_RTN_THRESH16   0x69A       /* Programmable Phase 3 retrain threshold 16 point */
#define MODEM_PHASE3_PRECORER       0xA1B       /* Phase 3 Precoder workaround */

/* V.34 Control channel parameters */
#define MODEM_V34_MPH_TX1           0x110
#define MODEM_V34_MPH_TX2           0x111
#define MODEM_V34_MPH_TX3           0x112
#define MODEM_V34_MPH_TX4           0x113
#define MODEM_V34_MPH_RX1           0x374
#define MODEM_V34_MPH_RX2           0x375
#define MODEM_V34_MPH_RX3           0x376
#define MODEM_V34_MPH_RX4           0x377
#define MODEM_V34_CCR_ENABLE        0x13F       /* Control Channel Rates Enable (bit 6) */
#define MODEM_V34_CC_RX_RATE        0x6DD       /* Control Channel receive data rate (bit 2) */
#define MODEM_V34_CC_TX_RATE        0x6DD       /* Control Channel transmit data rate (bit 3) */
#define MODEM_V34_AUTO_HDX_CC_SEL   0x6CC       /* Automatic Selection of V.34 HDX Control Channel (bit 6) */

/* V.34 Data rate selection procedures */
#define MODEM_V34_DATA_RATE_H       0x383   /* V.34 data rate mask */
#define MODEM_V34_DATA_RATE_L       0x382   /* V.34 data rate mask */
#define MODEM_V34_2400              0x0001
#define MODEM_V34_4800              0x0002
#define MODEM_V34_7200              0x0004
#define MODEM_V34_9600              0x0008
#define MODEM_V34_12000             0x0010
#define MODEM_V34_14400             0x0020
#define MODEM_V34_16800             0x0040
#define MODEM_V34_19200             0x0080
#define MODEM_V34_21600             0x0100
#define MODEM_V34_24000             0x0200
#define MODEM_V34_26400             0x0400
#define MODEM_V34_28800             0x0800
#define MODEM_V34_31200             0x1000
#define MODEM_V34_33600             0x2000
#define MODEM_V34_REM_DATA_RATE_H   0x209   /* V.34 Remote Data Rate Capability */
#define MODEM_V34_REM_DATA_RATE_L   0x208   /* V.34 Remote Data Rate Capability */
#define MODEM_V34_CURR_TX_SPEED     0x2E5
#define MODEM_V34_CURR_RX_SPEED     0x2E4
#define MODEM_V34_CURR_336          0x0E
#define MODEM_V34_CURR_312          0x0D
#define MODEM_V34_CURR_288          0x0C
#define MODEM_V34_CURR_264          0x0B
#define MODEM_V34_CURR_240          0x0A
#define MODEM_V34_CURR_216          0x09
#define MODEM_V34_CURR_192          0x08
#define MODEM_V34_CURR_168          0x07
#define MODEM_V34_CURR_144          0x06
#define MODEM_V34_CURR_120          0x05
#define MODEM_V34_CURR_96           0x04
#define MODEM_V34_CURR_72           0x03
#define MODEM_V34_CURR_48           0x02
#define MODEM_V34_CURR_24           0x01

#define MODEM_V34_ARA_RAM_ENABLE    0x3A5       /* ARA in RAM Enable (bit 4) */
#define MODEM_V34_ARA_RAM_THRESH    0x3B0       /* 3B0 - 3BE */
#define MODEM_V34_ARA_EQM_BIAS1     0x2A4       /* ARA EQM Bias Threshold 1 */
#define MODEM_V34_ARA_EQM_BIAS2     0x2A5       /* ARA EQM Bias Threshold 2 */


/* V.17, V.29, and V.27 functions */
#define MODEM_PNSUC_QUAL_EQM_ENB    0x6D7       /* PNSUC qualified by EQM Enable (bit 2) */
#define MODEM_PNSUC_QUAL_EQM_THRSH  0x6D8       /* PNSUC qualified by EQM threshold */
#define MODEM_FORCE_TRAIN_MODE      0x6CC       /* Forced train mode (bit 3) */
#define MODEM_TCF_FXN_ENB           0x6D7       /* TCF function enable */
#define MODEM_TCF_FXN_ENB_BIT       0x80
#define MODEM_TCF_DURATION_TIMERH   0x6C5       /* TCF duration timer */
#define MODEM_TCF_DURATION_TIMERL   0x6C4       /* TCF duration timer */
#define MODEM_TX_PIPE_EMPTY         0x00C       /* Transmitter pipe empty */
#define MODEM_CARRIER_FREQ_1700     0x3C1       /* Carrier Frequency 1700 Hz (bit 3) */
#define MODEM_CTS_MARK_QUALIFY      0x10D       /* V.21/V.23 CTS MArk Qualify (bit 3) */
#define MODEM_TX_CED_ENABLE         0x6D7       /* Transmit CED enable (bit 6) */
#define MODEM_CED_TIMERL            0x6C4       /* Tramsmit CED Timer lo byte */
#define MODEM_CED_TIMERH            0x6C5       /* Transmit CED Timer hi byte */


#define MODEM_HDLC_STATUS_MASK      0xC6

/* IA shadow registers: with index from 0 to 3 for the primary IA, 4 to 7 for the secondary IA */
#define MODEM_IA_BASE_ADDR          0x3C8       /* IA control registers base address */
#define VOICE_IA_BASE_REG           4           /* Secondary IA registers index offset */

/* Primary (Modem) IA shadow registers: */
#define MODEM_IA1_CONTROL0_LO       0x3C8       /* Primary IA control register 00 low byte */
#define MODEM_IA1_CONTROL0_HI       0x3C9       /* Primary IA control register 00 high byte */
#define MODEM_IA1_CONTROL1_LO       0x3CA       /* Primary IA control register 01 low byte */
#define MODEM_IA1_CONTROL1_HI       0x3CB       /* Primary IA control register 01 high byte */
#define MODEM_IA1_CONTROL2_LO       0x3CC       /* Primary IA control register 10 low byte */
#define MODEM_IA1_CONTROL2_HI       0x3CD       /* Primary IA control register 10 high byte */
#define MODEM_IA1_CONTROL3_LO       0x3CE       /* Primary IA control register 11 low byte */
#define MODEM_IA1_CONTROL3_HI       0x3CF       /* Primary IA control register 11 high byte */
/* Secondary (Voice) IA shadow registers: */
#define MODEM_IA2_CONTROL0_LO       0x3D0       /* Secondary IA control register 00 low byte */
#define MODEM_IA2_CONTROL0_HI       0x3D1       /* Secondary IA control register 00 high byte */
#define MODEM_IA2_CONTROL1_LO       0x3D2       /* Secondary IA control register 01 low byte */
#define MODEM_IA2_CONTROL1_HI       0x3D3       /* Secondary IA control register 01 high byte */
#define MODEM_IA2_CONTROL2_LO       0x3D4       /* Secondary IA control register 10 low byte */
#define MODEM_IA2_CONTROL2_HI       0x3D5       /* Secondary IA control register 10 high byte */
#define MODEM_IA2_CONTROL3_LO       0x3D6       /* Secondary IA control register 11 low byte */
#define MODEM_IA2_CONTROL3_HI       0x3D7       /* Secondary IA control register 11 high byte */


/* Modem State Definitions */
#define MODEM_IDLE          0
#define HDLC_RX_MODE        1
#define HDLC_TX_MODE        2
#define T4_RX_MODE          3
#define T4_TX_MODE          4
#define FLAGS_RX_MODE       5
#define CID_RX_MODE         6   /* Caller Id. (V.23 Receive only) */
#define MODEM_V8_MODE       7   /* V34 handshake (negotiation) states */
#define MODEM_TX_TCF        8   /* Tx TCF */
#define MODEM_RX_TCF        9   /* Rx TCF */
#define VOICE_RX_MODE       10
#define VOICE_TX_MODE       11
#define MODEM_SLEEP         12

#define MODEM_DELAY_TIME    20

extern void ModemSWIsr (void);
extern void ModemBadIsr(void);
extern void ModemRxHDLCNewStatusIsr(void);
extern void ModemTxHDLCNewStatusIsr(void);
extern void ModemRxFlagsIsr (void);
extern void ModemRxHDLCIsr(void);
extern void ModemTxHDLCIsr(void);
extern void ModemRxT4Isr(void);
extern void ModemTxT4Isr(void);
extern void ModemRxVoiceIsr(void);
extern void ModemTxVoiceIsr(void);
extern void ModemTxTCFIsr(void);        //THY
extern void ModemRxTCFIsr(void);        // THY

extern void ModemCDBWriteByte(UINT8);
extern UINT16 ModemCDBReadLength(void);
extern UINT8 ModemCDBReadByte(void);
extern void ModemCDBTask(void);
extern void ModemIRQTask(UINT32 arg);
extern void ModemReset(UINT8 force);
extern void ModemInit(void);
extern void ModemSetNewMaxGain(void);
extern void ModemRxConfig(UINT8, UINT8);
extern void ModemRxConfigHiSpeed(UINT8);
extern void ModemTxConfig(UINT8, UINT8);
extern void ModemTxConfigHDLC(UINT8);
extern void ModemTxConfigHiSpeed(UINT8);
extern void ModemTxConfigVoice(UINT8,UINT8,UINT8);
extern void ModemTxSetV34SymbolRate( UINT16 rate );
extern void ModemTxSetPermanentData( UINT8 configure, UINT8 data, UINT8 speed, UINT8 training );
extern UINT8 ModemCompareAGCGain(void);
extern void ModemConfig(UINT8);
extern UINT16 ModemReadAGCGain(void);
extern void ModemSetDTMFOnTime(INT16);
extern void ModemSetFlagCnt(UINT16);
extern void ModemSetMaxGain(UINT16);
extern void ModemSetNewMaxGain(void);
extern void ModemTxSetLevel(UINT16);
extern void ModemTxSetToneLevel(void);
extern void ModemTxSetDTMFTime( UINT16 tone_on_time, UINT16 tone_off_time );
extern void ModemTxSetDTMFLevle( UINT8 digit, UINT8 lband_level, UINT8 hband_level );
extern void ModemSetVoiceAGC(UINT8);
extern void ModemSetVoiceIntRate(void);
extern void ModemSetVoiceSpeed(UINT8);
extern void ModemSetVoiceThreshold(void);
extern void ModemTxFinishInitPage (void);
extern void ModemTxDataStop (void);
extern void ModemTxSwitchPCtoCC( void );
extern UINT8 ModemRxSwitchPCtoCC( void );
extern UINT8 ModemTxWaitForFIFOEmpty( UINT8 efifo );
extern UINT8 ModemTxWaitForCC( UINT8 ctrl );
extern UINT8 ModemRxWaitForCC( UINT8 ctrl );
extern UINT8 ModemRxWaitForPC( UINT8 ctrl );
extern const UINT16 ModemRxMaxGainTbl[];
extern const UINT16 DeltaMaxGain;
extern const UINT8  VoiceECM;
extern const UINT16 VOXOnThresh;
extern const UINT16 VOXOffThresh;
extern const UINT16 TxToneTbl[];
extern const float  TxDTMFTbl[];
extern const UINT16 TxSpkrToneTbl[];
extern const UINT16 TxLevelTbl[];
extern const UINT8  ModemConfigTbl[];
extern const UINT16 *FreqAccTable[];
extern const UINT16 FreqAccTbl1[];
extern const UINT16 FreqAccTbl2[];
extern const UINT16 FreqAccTbl3[];
extern const UINT8  LossTbl[];
extern const UINT8  SpeedMaskTbl[];
extern const UINT8  ToneCntrlTbl[];
extern const UINT8  ToneAccTbl[];
extern const UINT8  SpkrToneCntrlTbl[];
extern const UINT8  SpkrToneAccTbl[];
extern const UINT16 ModemSpeedTbl[];
extern const UINT16 V34SpeedTbl[];
extern const UINT16 V17SpeedTbl[];

extern const DTMF_COMP_LEVEL DTMFCompLevel[];
extern const TX_COMP_LEVEL   TxCompLevel[];

extern UINT8  ModemDSPRead1(UINT16 addr);
extern UINT16 ModemDSPRead2(UINT16 addrH, UINT16 addrL);
extern UINT16 ModemDSPRead4(UINT16 addr);
extern UINT16 ModemDSPRead6(UINT16 addrH, UINT16 addrL);
extern UINT32 ModemDSPRead7(UINT16 addrH, UINT16 addrL);
extern void   ModemDSPWrite1(UINT16 addr,  UINT8  data,  BOOL bitset_only);
extern void   ModemDSPWrite2(UINT16 addrH, UINT16 addrL, UINT16 data);
extern void   ModemDSPWrite3(UINT16 addr,  UINT16 data);
extern UINT16 ModemDSPWrite5(UINT16 addr,  UINT16 Resetbits, UINT16 Setbits);
extern UINT8 ModemV8StatusIsr(void);
extern UINT8 ModemIoReadByte(UINT8 addr);
extern void  ModemIoWriteByte(UINT8 addr, UINT8 byte);
extern BOOL  ModemIoBitClear(UINT8 addr, UINT8 bits);
extern BOOL  ModemIoBitSet(UINT8 addr, UINT8 bits);
extern void  ModemIoSetBit(UINT8 addr, UINT8 bits);
extern void  ModemIoClearBit(UINT8 addr, UINT8 bits);
extern void  mdm_spi_multi_read( UINT8 addr, UINT8 *data, UINT8 cnt );
extern void  mdm_spi_multi_write( UINT8 addr, UINT8 *data, UINT8 cnt );


