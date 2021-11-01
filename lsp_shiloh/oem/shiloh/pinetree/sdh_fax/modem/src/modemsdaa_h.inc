/*
 *  $RCSfile: modemsdaa_h.inc,v $
 * $Revision: 1.8 $
 *   $Author: miurat $
 *     $Date: 2012/03/21 00:27:23 $
 */
/************************************************************************
 *
 *      modemsdaa_h.inc
 *
 *  SSF336 SDAA driver.
 *
 *  Copyright 2007 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef _SDAA_H
#define _SDAA_H


/*
 SSD registers
 --------------------
 */

/* -------- [ 0x20 ] ----------- */
#define  GREG_SOFT_CFG               0x2F
#define  GREG_CTRL0                  0x2E
#define  GREG_CTRL1                  0x2D
#define  GREG_IMSR                   0x2A
#define  GREG_IER                    0x29
#define  GREG_ISR                    0x28
#define  GREG_DIR                    0x26
#define  GREG_OUTPUT_DATA            0x25
#define  GREG_INPUT_DATA             0x24
#define  GREG_DRVSTR1                0x23
#define  GREG_DRVSTR0                0x22
#define  GREG_IRQ_POL                0x21
#define  GREG_IRQ_TYPE               0x20


/*
 GPIO masks
 --------------------
 */

#define  GREG_EX_RELAY               0x10
#define  GREG_HS_RESET               0x01
#define  GREG_VOL0                   0x02
#define  GREG_VOL1                   0x04
#define  GREG_VOL2                   0x08
#define  GREG_MASK                   0x1F
#define  GREG_HS_STATUS              0x02


/*
 * HDP Control Bit format: (For single bit fields)
 * Bits 0-15  = DSP RAM address
 * Bits 16-23 = The bit number
 * Bits 24-31 = 8 bit mask of the bit
 */
#define  HDPRAMCTLBIT(Reg, Bit)      ( UINT32 )( Reg | ((Bit)<<16) | ((1<<(Bit))<<24) )

#define  API_ENABLE                  HDPRAMCTLBIT( 0xD6, 3 )
#define  API_SELECT_EXTERNAL         HDPRAMCTLBIT( 0x00, 5 )
#define  API_SSD_INTERRUPT           HDPRAMCTLBIT( 0x4C0, 6 )

#define  API_STA_TNRVOLT             HDPRAMCTLBIT( 0x720, 0 )
#define  API_STA_CURRENT             HDPRAMCTLBIT( 0x720, 1 )
#define  API_STA_LINERES             HDPRAMCTLBIT( 0x720, 2 )
#define  API_STA_LSDVDD              HDPRAMCTLBIT( 0x720, 3 )
#define  API_STA_DIGITAL             HDPRAMCTLBIT( 0x720, 4 )
#define  API_STA_HOOKDONE            HDPRAMCTLBIT( 0x720, 5 )
#define  API_STA_LINEINUSE           HDPRAMCTLBIT( 0x720, 6 )
#define  API_STA_NOLINE              HDPRAMCTLBIT( 0x720, 7 )
#define  API_STA_LINELOSS            HDPRAMCTLBIT( 0x721, 0 )
#define  API_STA_ACHIGH              HDPRAMCTLBIT( 0x721, 1 )
#define  API_STA_DCHIGH              HDPRAMCTLBIT( 0x721, 2 )
#define  API_STA_SSDFAIL             HDPRAMCTLBIT( 0x721, 3 )
#define  API_STA_LSDFAIL             HDPRAMCTLBIT( 0x721, 4 )
#define  API_STA_RING                HDPRAMCTLBIT( 0x721, 5 )
#define  API_STA_EXTENSION           HDPRAMCTLBIT( 0x721, 6 )
#define  API_STA_LSDFAULT            HDPRAMCTLBIT( 0x721, 7 )
#define  API_STA_PULSEDONE           HDPRAMCTLBIT( 0x722, 0 )
#define  API_STA_LINEREVERSAL        HDPRAMCTLBIT( 0x722, 1 )
#define  API_STA_DIBERROR            HDPRAMCTLBIT( 0x722, 2 )
#define  API_STA_DIBERRORLIMIT       HDPRAMCTLBIT( 0x722, 3 )
#define  API_STA_BRAZILRING          HDPRAMCTLBIT( 0x722, 6 )
#define  API_STA_RINGDETECTED        HDPRAMCTLBIT( 0x749, 0 )
#define  API_STA_RINGABOVETHRESHOLD  HDPRAMCTLBIT( 0x749, 1 )

#define  API_SEL_FASTEDGEPULSEDIAL   HDPRAMCTLBIT( 0x74F, 0 )
#define  API_SEL_RINGREGARDLESSLIU   HDPRAMCTLBIT( 0x74F, 1 )
#define  API_SEL_RINGREGARLESSNOLINE HDPRAMCTLBIT( 0x74F, 2 )
#define  API_SEL_RINGSTATUSMODE      HDPRAMCTLBIT( 0x74F, 3 )
#define  API_SEL_KOREARINGIMPEDANCE  HDPRAMCTLBIT( 0x74F, 4 )
#define  API_SEL_BRAZILRINGENABLE    HDPRAMCTLBIT( 0x74F, 5 )
#define  API_SEL_BONDOPTIONDISABLE   HDPRAMCTLBIT( 0x74F, 6 )

#define  API_RingThreshold           0x730
#define  API_Complex                 0x731
#define  API_DCCurve                 0x733
#define  API_RMake                   0x734
#define  API_BreakTime               0x73E
#define  API_MakeTime                0x73F

#define  API_DC0VoltagePoint0        0x7B0
#define  API_DC0VoltagePoint1        0x7B1
#define  API_DC0VoltagePoint2        0x7B2
#define  API_DC0VoltagePoint3        0x7B3
#define  API_DC0VoltagePoint4        0x7B4
#define  API_DC0CurrentPoint0        0x7B5
#define  API_DC0CurrentPoint1        0x7B6
#define  API_DC0CurrentPoint2        0x7B7
#define  API_DC0CurrentPoint3        0x7B8
#define  API_DC0CurrentPoint4        0x7B9

#define  API_InitialDACforDC0        0x7BA
#define  API_InitialDACforDC1        0x7BB
#define  API_InitialDACforDC2        0x7BC
#define  API_InitialDACforDC3        0x7BD
#define  API_InitialDACforDC4        0x7BE

#define  API_ExtRing                 0x741
#define  API_CodeRev                 0x745
#define  API_LSDVersion              0x746
#define  API_HookState               0x747
#define  API_RingMin                 0x74A
#define  API_RingMax                 0x74B
#define  API_PulseSetup              0x74C
#define  API_PulseClear              0x74D
#define  API_PulseInterDigit         0x74E

#define  API_TnRVoltageLo            0x728
#define  API_TnRVoltageHi            0x729
#define  API_fCurrentLo              0x72A
#define  API_fCurrentHi              0x72B
#define  API_fRsLo                   0x72C
#define  API_fRsHi                   0x72D
#define  API_LSDVDDLo                0x72E
#define  API_LSDVDDHi                0x72F
#define  API_fVBattLo                0x76B
#define  API_fVBattHi                0x76C

#define  API_LineInUseThreshold_H    0x75D
#define  API_LineInUseThreshold_L    0x75C
#define  API_ExtensionThreshold_H    0x75F
#define  API_ExtensionThreshold_L    0x75E
#define  API_DigitalThreshold_H      0x763
#define  API_DigitalThreshold_L      0x762

#define  API_TaskCommand             0x750
#define  API_TaskParmA               0x751
#define  API_TaskParmB               0x752

#define  API_CMD_INIT                0x01
#define  API_CMD_FLASHHOOK           0x04
#define  API_CMD_TEST                0x09
#define  API_CMD_PULSEDIAL           0x0C
#define  API_CMD_GETLSDPOWER         0x0D
#define  API_CMD_DACMUTE             0x15
#define  API_CMD_ADCMUTE             0x16
#define  API_CMD_HOOKSTATE           0x17
#define  API_CMD_READSSDLSD          0x18
#define  API_CMD_WRITESSDLSD         0x19
#define  API_CMD_PULSEDIALDIGIT      0x1A
#define  API_CMD_INITRINGTHRESHOLD   0x1B
#define  API_CMD_DISABLEMONITOR      0x24
#define  API_CMD_ENABLEMONITOR       0x25
#define  API_CMD_VOLUME              0x27
#define  API_CMD_SPEAKERSOURCE       0x28
#define  API_CMD_EXTENSION           0x29
#define  API_CMD_OFFHOOKMONITOR      0x2A

#define  API_VOLUMEPARM_OFF          0
#define  API_VOLUMEPARM_LOW          1
#define  API_VOLUMEPARM_MEDIUM       2
#define  API_VOLUMEPARM_HIGH         3

#define  API_SPEAKERPARM_DAC         0
#define  API_SPEAKERPARM_PWM         1

#define  API_HOOKPARM_OFFHOOK        2
#define  API_HOOKPARM_ONHOOK_DATA    3
#define  API_HOOKPARM_ONHOOK_STATUS  4
#define  API_HOOKPARM_PULSE_MAKE     5
#define  API_HOOKPARM_PULSE_BREAK    6
#define  API_HOOKPARM_PULSE_SETUP    7
#define  API_HOOKPARM_PULSE_CLEAR    8
#define  API_HOOKPARM_PULSE_CLEANUP  9

#define  API_OFFHOOKMONITOR_OFF      0
#define  API_OFFHOOKMONITOR_ON       1

#define  API_PULSEDIAL_MAKE          0
#define  API_PULSEDIAL_BREAK         1
#define  API_PULSEDIAL_CLEAR         2
#define  API_PULSEDIAL_SETUP         3
#define  API_PULSEDIAL_CLEANUP       4

#define  API_EXTENSIONPARM_BOTHHOOK  3


typedef enum
{
	Z600OHM,
	COMPLEX,
	Z540OHM
} SDAA_IMPEDANCE;

typedef enum
{
	SDAA_DC0,
	SDAA_DC1,
	SDAA_DC2,
	SDAA_DC3,
	SDAA_DC4
} SDAA_DC_CURVE;

typedef enum
{
	SDAA_MAKE_OFFHOOK,
	SDAA_MAKE_100,
	SDAA_MAKE_200,
	SDAA_MAKE_300,
	SDAA_MAKE_400
} SDAA_PULSE_OHM;

typedef enum
{
	SDAA_PULSE_SLOWEDGE,
	SDAA_PULSE_FASTEDGE
} SDAA_PULSE_EDGE;

typedef struct tagCountryInfo
{
	UINT8  LineImpedance;
	UINT8  DCCurve;
	UINT8  RMake;
	UINT8  PulseEdge;
	UINT8  InitialDACvalue;
	UINT8  RingMaxFrequency;
	UINT8  RingMinFrequency;
	UINT8  RingThreshold;
	UINT8  SouthKoreaRingImpedance;
	UINT8  BrazilRingDetection;
	UINT8  LineInUseThreshold;
	UINT8  ExtensionThreshold;
	UINT8  DigitalThreshold;
	UINT8  DIBPower;
} SDAA_COUNTRY_INFO, *PSDAA_COUNTRY_INFO;

typedef enum tagSPKRsource
{
	SDAA_SPKR_RX,
	SDAA_SPKR_TX,
	SDAA_SPKR_RXTX
} SDAA_SPKR_SOURCE, *PSDAA_SPKR_SOURCE;

typedef enum tagVolLevel
{
	SDAA_VOLUME_OFF,
	SDAA_VOLUME_LOW,
	SDAA_VOLUME_MED,
	SDAA_VOLUME_HIGH
} SDAA_VOLUME_LEVEL, *PSDAA_VOLUME_LEVEL;

#define SDAA_DC_POINTS 5

typedef struct tagSDAAdata
{
	UINT8               T35Code;
	SDAA_COUNTRY_INFO   m_country;
	UINT8               m_DC0VoltagePoint[ SDAA_DC_POINTS ];
	UINT8               m_DC0CurrentPoint[ SDAA_DC_POINTS ];
	UINT8               RefTRVoltage;
	UINT8               RefLineInUseVoltage;
	UINT8               RefLineInUseDelta;
} SDAADATA, *PSDAADATA;

/* GENERAL API */

void  SSFInitParams( CtryPrmsStruct *Ctry );
void  SSFOffHook( void );
void  SSFOnHook( void );
void  SSFPulseDial( UINT8 digit, UINT8 pulse_mode, UINT8 make_time, UINT8 break_time );
void  SSFPulseSetup( void );
void  SSFPulseCleanup( void );
UINT8 SSFReadSSD( UINT8 cAddr );
void  SSFWriteSSD( UINT8 cAddr, UINT8 cData );
UINT8 SSFReadLSD( UINT8 cAddr );
void  SSFWriteLSD( UINT8 cAddr, UINT8 cData );
BOOL  SSFSpeakerVolume( UINT8 Volume );
void  SSFStopMonitorAPI( void );
void  SSFStartMonitorAPI( void );

LINE_STATUS SSFLineInUse( void );
LINE_STATUS SSFLineStatus( void );

#endif // _SDAA_H

