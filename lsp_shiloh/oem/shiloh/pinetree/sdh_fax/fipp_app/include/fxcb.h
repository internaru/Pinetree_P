/** \file
*/
/* ************************************************************************* **
**                                                                           **
**                 Proprietary and Confidential Information                  **
**                                                                           **
**                Copyright (c) 2002 Oasis Semiconductor Inc.                **
**                                                                           **
**  This software contains proprietary and confidential material which is    **
**  property of Oasis Semiconductor Inc.                                     **
**                                                                           **
** ************************************************************************* */
/*
	File: fxcb.h
	Date: May 6, 2002
	By:   S.J. Busak

	Contains the definition of the interface between the User Interface Unit (UIU)
	and the Fax Scheduler Uint (FSU).

*/

#ifndef __FXCB_H__
	#define __FXCB_H__

	#include "dc_type.h"
  #include "fshs.h"

  //#ifdef PROJECT_SY  // -sjb-12/17/08  +sjb-6/9/05
  #if 0  // +sjb-12/17/08
    #include "fsu_sy.h"
  #endif  // +sjb-6/9/05: End

#if 0
/// Vertical Resolution. Used by control block member \c vert_res.
enum  // Values for fxcb.to_fsu.parm.vert_res
{
	FAX_VR_STANDARD = 0,  //!< R8x3.85 l/mm
	FAX_VR_FINE,      //!< R8x7.70 l/mm
	FAX_VR_SUPERFINE, //!< R8x15.4 l/mm
	FAX_VR_300,       //!< 300 x 300
	FAX_VR_ULTRAFINE  //!< 400 x 400 (R16x15.4 l/mm)
};
#endif

/// Error correction mode setting.
/// Used by the control block member \c ecm_setting.
enum  // Values for fxcb.to_fsu.parm.ecm_setting:
{
	T30_EC_NO_ECM,  //!< Transmit/receive with using ECM
	T30_EC_256,     //!< Transmit/receive using 64K ECM buffer
	T30_EC_64       //!< Transmit/receive using 16K ECM buffer
};

/// Fax color type for transmission.
/// Used by control block's \c fax_type.
enum  // Values for fxcb.to_fsu.parm.fax_type:
{
	FAX_TYPE_MONO,          //!< Mono
	FAX_TYPE_GRAYSCALE,     //!< Gray-scale
	FAX_TYPE_COLOR,         //!< Color 
  FAX_TYPE_MONO_PHOTO           //!< .
};

  #define DAA_CODE_JAPAN                                 0x00
  #define DAA_CODE_ALBANIA                               0x01
  #define DAA_CODE_ALGERIA                               0x02
  #define DAA_CODE_AMERICAN_SAMOA                        0x03
  #define DAA_CODE_GERMANY                               0x04 
  #define DAA_CODE_ANGUILLA                              0x05
  #define DAA_CODE_ANTIGUA_AND_BARBUDA                   0x06
  #define DAA_CODE_ARGENTINA                             0x07
  #define DAA_CODE_ASCENSION                             0x08
  #define DAA_CODE_AUSTRALIA                             0x09
  #define DAA_CODE_AUSTRIA                               0x0A
  #define DAA_CODE_BAHAMAS                               0x0B
  #define DAA_CODE_BAHRAIN                               0x0C
  #define DAA_CODE_BANGLADESH                            0x0D
  #define DAA_CODE_BARBADOS                              0x0E
  #define DAA_CODE_BELGIUM                               0x0F

  #define DAA_CODE_BELIZE                                0x10
  #define DAA_CODE_BENIN                                 0x11
  #define DAA_CODE_BERMUDA                               0x12
  #define DAA_CODE_BHUTAN                                0x13
  #define DAA_CODE_BOLIVIA                               0x14
  #define DAA_CODE_BOTSWANA                              0x15
  #define DAA_CODE_BRAZIL                                0x16
  #define DAA_CODE_BRITISH_ANTARCTIC_TERRITORY           0x17
  #define DAA_CODE_BRITISH_INDIAN_OCEAN_TERRITORY        0x18
  #define DAA_CODE_BRITISH_VIRGIN_ISLANDS                0x19
    #define DAA_CODE_VIRGINISLANDS                       0x19  // For backward campatibility
  #define DAA_CODE_BRUNEI_DARUSSALAM                     0x1A
    #define	DAA_CODE_BRUNEI                              0x1A  // For backward campatibility
  #define DAA_CODE_BULGARIA                              0x1B
  #define DAA_CODE_MYANMAR                               0x1C
  #define DAA_CODE_BURUNDI                               0x1D
  #define DAA_CODE_BELARUS                               0x1E
  #define DAA_CODE_CAMEROON                              0x1F

  #define DAA_CODE_CANADA                                0x20
  #define DAA_CODE_CAPE_VERDE                            0x21
  #define DAA_CODE_CAYMAN_ISLANDS                        0x22
  #define DAA_CODE_CENTRAL_AFRICAN_REPUBLIC              0x23
  #define DAA_CODE_CHAD                                  0x24
  #define DAA_CODE_CHILE                                 0x25
  #define DAA_CODE_CHINA                                 0x26
    #define DAA_CODE_PRC                                 0x26  // For backward campatibility
  #define DAA_CODE_COLOMBIA                              0x27
  #define DAA_CODE_COMOROS                               0x28
  #define DAA_CODE_CONGO                                 0x29
  #define DAA_CODE_COOK_ISLANDS                          0x2A
  #define DAA_CODE_COSTA_RICA                            0x2B
    #define DAA_CODE_COSTARICA                           0x2B  // For backward campatibility
  #define DAA_CODE_CUBA                                  0x2C
  #define DAA_CODE_CYPRUS                                0x2D
  #define DAA_CODE_CZECH_REPUBLIC                        0x2E
    #define DAA_CODE_CZECHREP                            0x2E  // For backward campatibility
  #define DAA_CODE_CAMBODIA                              0x2F

  #define DAA_CODE_DEMOCRATIC_PEOPLES_REPUBLIC_OF_KOREA  0x30
  #define DAA_CODE_DENMARK                               0x31
  #define DAA_CODE_DJIBOUTI                              0x32
  #define DAA_CODE_DOMINICAN_REPUBLIC                    0x33
  #define DAA_CODE_DOMINICA                              0x34
  #define DAA_CODE_ECUADOR                               0x35
  #define DAA_CODE_EGYPT                                 0x36
  #define DAA_CODE_EL_SALVADOR                           0x37
    #define	DAA_CODE_ELSALVADOR                          0x37  // For backward campatibility
  #define DAA_CODE_EQUATORIAL_GUINEA                     0x38
  #define DAA_CODE_ETHIOPIA                              0x39
  #define DAA_CODE_FALKLAND_ISLANDS                      0x3A
    #define	DAA_CODE_FALKLAND                            0x3A  // For backward campatibility
  #define DAA_CODE_FIJI                                  0x3B
  #define DAA_CODE_FINLAND                               0x3C
  #define DAA_CODE_FRANCE                                0x3D
  #define DAA_CODE_FRENCH_POLYNESIA                      0x3E
  #define DAA_CODE_NO_COUNTRY_DEFINED                    0x3F

  #define DAA_CODE_GABON                                 0x40
  #define DAA_CODE_GAMBIA                                0x41
  #define DAA_CODE_GERMANY_42                            0x42 
  #define DAA_CODE_ANGOLA                                0x43
  #define DAA_CODE_GHANA                                 0x44
  #define DAA_CODE_GIBRALTAR                             0x45
  #define DAA_CODE_GREECE                                0x46
  #define DAA_CODE_GRENADA                               0x47
  #define DAA_CODE_GUAM                                  0x48
  #define DAA_CODE_GUATEMALA                             0x49
  #define DAA_CODE_GUERNSEY                              0x4A
  #define DAA_CODE_GUINEA                                0x4B
  #define DAA_CODE_GUINEA_BISSAU                         0x4C
    #define	DAA_CODE_GUINEABISSAU                        0x4C  // For backward campatibility
  #define DAA_CODE_GUAYANA                               0x4D
    #define	DAA_CODE_GUYANA                              0x4D  // For backward campatibility
  #define DAA_CODE_HAITI                                 0x4E
  #define DAA_CODE_HONDURAS                              0x4F

  #define DAA_CODE_HONGKONG                              0x50
  #define DAA_CODE_HUNGARY                               0x51
  #define DAA_CODE_ICELAND                               0x52
  #define DAA_CODE_INDIA                                 0x53
  #define DAA_CODE_INDONESIA                             0x54
  #define DAA_CODE_IRAN                                  0x55
  #define DAA_CODE_IRAQ                                  0x56
  #define DAA_CODE_IRELAND                               0x57
  #define DAA_CODE_ISRAEL                                0x58
  #define DAA_CODE_ITALY                                 0x59
  #define DAA_CODE_COTE_DIVOIRE                          0x5A
  #define DAA_CODE_JAMAICA                               0x5B
  #define DAA_CODE_AFGHANISTAN                           0x5C
  #define DAA_CODE_JERSEY                                0x5D
  #define DAA_CODE_JORDAN                                0x5E
  #define DAA_CODE_KENYA                                 0x5F

  #define DAA_CODE_KIRIBATI                              0x60
  #define DAA_CODE_KOREA                                 0x61
  #define DAA_CODE_KUWAIT                                0x62
  #define DAA_CODE_LAO_PEOPLES_DEMOCRATIC_REPUBLIC       0x63
  #define DAA_CODE_LEBANON                               0x64
  #define DAA_CODE_LESOTHO                               0x65
  #define DAA_CODE_LIBERIA                               0x66
  #define DAA_CODE_LIBYAN_ARAB_JAMAHIRIYA                0x67
  #define DAA_CODE_LIECHTENSTEIN                         0x68
  #define DAA_CODE_LUXEMBOURG                            0x69
  #define DAA_CODE_MACAO                                 0x6A
  #define DAA_CODE_MADAGASCAR                            0x6B
  #define DAA_CODE_MALAYSIA                              0x6C
  #define DAA_CODE_MALAWI                                0x6D
  #define DAA_CODE_MALDIVES                              0x6E
  #define DAA_CODE_MALI                                  0x6F

  #define DAA_CODE_MALTA                                 0x70
  #define DAA_CODE_MAURITANIA                            0x71
  #define DAA_CODE_MAURITIUS                             0x72
  #define DAA_CODE_MEXICO                                0x73
  #define DAA_CODE_MONACO                                0x74
  #define DAA_CODE_MONGOLIA                              0x75
  #define DAA_CODE_MONTSERRAT                            0x76
  #define DAA_CODE_MOROCCO                               0x77
  #define DAA_CODE_MOZAMBIQUE                            0x78
  #define DAA_CODE_NAURU                                 0x79
  #define DAA_CODE_NEPAL                                 0x7A
  #define DAA_CODE_NETHERLANDS                           0x7B
  #define DAA_CODE_NETHERLANDS_ANTILLES                  0x7C
  #define DAA_CODE_NEW_CALEDONIA                         0x7D
  #define DAA_CODE_NEW_ZEALAND                           0x7E
    #define DAA_CODE_NEWZEALAND                          0x7E  // For backward campatibility
  #define DAA_CODE_NICARAGUA                             0x7F

  #define DAA_CODE_NIGER                                 0x80
  #define DAA_CODE_NIGERIA                               0x81
  #define DAA_CODE_NORWAY                                0x82
  #define DAA_CODE_OMAN                                  0x83
  #define DAA_CODE_PAKISTAN                              0x84
  #define DAA_CODE_PANAMA                                0x85
  #define DAA_CODE_PAPUA_NEW_GUINEA                      0x86
  #define DAA_CODE_PARAGUAY                              0x87
  #define DAA_CODE_PERU                                  0x88
  #define DAA_CODE_PHILIPPINES                           0x89
  #define DAA_CODE_POLAND                                0x8A
  #define DAA_CODE_PORTUGAL                              0x8B
  #define DAA_CODE_PUERTO_RICO                           0x8C
    #define DAA_CODE_PERTORICO                           0x8C  // For backward campatibility
  #define DAA_CODE_QATAR                                 0x8D
  #define DAA_CODE_ROMANIA                               0x8E
  #define DAA_CODE_RWANDA                                0x8F

  #define DAA_CODE_SAINT_KITTS_AND_NEVIS                 0x90
  #define DAA_CODE_SAINT_CROIX                           0x91
  #define DAA_CODE_SAINT_HELENA_AND_ASCENSION            0x92
  #define DAA_CODE_SAINT_LUCIA                           0x93
  #define DAA_CODE_SAN_MARINO                            0x94
  #define DAA_CODE_SAO_TOME_AND_PRINCIPE                 0x96
  #define DAA_CODE_SAINT_VINCENT_AND_THE_GRENADINES      0x97
  #define DAA_CODE_SAUDI_ARABIA                          0x98
    #define DAA_CODE_SAUDIARABIA                         0x98  // For backward campatibility
  #define DAA_CODE_SENEGAL                               0x99
  #define DAA_CODE_SEYCHELLES                            0x9A
  #define DAA_CODE_SIERRA_LEONE                          0x9B
  #define DAA_CODE_SINGAPORE                             0x9C
  #define DAA_CODE_SOLOMON_ISLANDS                       0x9D
  #define DAA_CODE_SOMALIA                               0x9E
  #define DAA_CODE_SOUTH_AFRICA                          0x9F
    #define DAA_CODE_SOUTHAFRICA                         0x9F  // For backward campatibility

  #define DAA_CODE_SPAIN                                 0xA0
  #define DAA_CODE_SRI_LANKA                             0xA1
    #define DAA_CODE_SRILANKA                            0xA1  // For backward campatibility
  #define DAA_CODE_SUDAN                                 0xA2
  #define DAA_CODE_SURINAME                              0xA3
  #define DAA_CODE_SWAZILAND                             0xA4
  #define DAA_CODE_SWEDEN                                0xA5
  #define DAA_CODE_SWITZERLAND                           0xA6
  #define DAA_CODE_SYRIAN_ARAB_REPUBLIC                  0xA7
    #define	DAA_CODE_SYRIA                               0xA7  // For backward campatibility
  #define DAA_CODE_TANZANIA                              0xA8
  #define DAA_CODE_THAILAND                              0xA9
  #define DAA_CODE_TOGO                                  0xAA
  #define DAA_CODE_TONGA                                 0xAB
  #define DAA_CODE_TRINIDAD_AND_TOBAGO                   0xAC
  #define DAA_CODE_TUNISIA                               0xAD
  #define DAA_CODE_TURKEY                                0xAE
  #define DAA_CODE_TURKS_AND_CAICOS_ISLANDS              0xAF

  #define DAA_CODE_TUVALU                                0xB0
  #define DAA_CODE_UGANDA                                0xB1
  #define DAA_CODE_UKRAINE                               0xB2
  #define DAA_CODE_UNITED_ARAB_EMIRATES                  0xB3
    #define DAA_CODE_UAE                                 0xB3  // For backward campatibility
  #define DAA_CODE_UNITED_KINGDOM                        0xB4
    #define DAA_CODE_UK                                  0xB4  // For backward campatibility
  #define DAA_CODE_UNITED_STATES                         0xB5
    #define DAA_CODE_NA                                  0xB5  // For backward campatibility
  #define DAA_CODE_BURKINA_FASO                          0xB6
  #define DAA_CODE_URUGUAY                               0xB7
  #define DAA_CODE_RUSSIAN_FEDERATION                    0xB8
    #define DAA_CODE_RUSSIA                              0xB8  // For backward campatibility
  #define DAA_CODE_VANUATU                               0xB9
  #define DAA_CODE_VATICAN                               0xBA
  #define DAA_CODE_VENEZUELA                             0xBB
  #define DAA_CODE_VIET_NAM                              0xBC
    #define DAA_CODE_VIETNAM                             0xBC  // For backward campatibility
  #define DAA_CODE_WALLIS_AND_FUTUNA                     0xBD
  #define DAA_CODE_SAMOA                                 0xBE
  #define DAA_CODE_YEMEN_BF                              0xBF

  #define DAA_CODE_YEMEN_C0                              0xC0
    #define	DAA_CODE_YEMEN                               0xC0  // For backward campatibility
  #define DAA_CODE_SERBIA                                0xC1
  #define DAA_CODE_DEMOCRATIC_REPUBLIC_OF_THE_CONGO      0xC2
  #define DAA_CODE_ZAMBIA                                0xC3
  #define DAA_CODE_ZIMBABWE                              0xC4
  #define DAA_CODE_SLOVAKIA                              0xC5
  #define DAA_CODE_SLOVENIA                              0xC6
  #define DAA_CODE_LITHUANIA                             0xC7
  #define DAA_CODE_MONTENEGRO                            0xC8

  #define DAA_CODE_CARIBBEAN                             0xF3  // For backward campatibility
  #define	DAA_CODE_ERITREA                               0xF4  // For backward campatibility
  #define	DAA_CODE_CANARYISLAND                          0xF5  // For backward campatibility
  #define	DAA_CODE_CABOVERDE                             0xF6  // For backward campatibility
  #define	DAA_CODE_MACEDONIA                             0xF7  // For backward campatibility
  #define	DAA_CODE_BOSNIA                                0xF8  // For backward campatibility
  #define	DAA_CODE_ANDORRA                               0xF9  // For backward campatibility
  #define DAA_CODE_LATVIA                                0xFA  // For backward campatibility
  #define DAA_CODE_ESTONIA                               0xFB  // For backward campatibility
  #define DAA_CODE_CROATIA                               0xFC  // For backward campatibility
  #define DAA_CODE_EUROPE                                0xFD  // For backward campatibility
  #define DAA_CODE_TAIWAN_REPUBLIC_OF_CHINA              0xFE
    #define DAA_CODE_TAIWAN                              0xFE  // For backward campatibility

// Limit for fxcb.to_fsu.parm.fax_forward_num:
/// Forward faxes to this number.
/// Does not include null terminator.
/// Used by control block's \c fax_forward_num.
//#define FAX_FORWARD_NUM_MAX 64 //32  // -sjb-6/9/05

// ------------------------------------------------- //
// Definitions for Commands from the UIU to the FSU: //
// ------------------------------------------------- //
enum
{
	FSU_CMD_NULL,            // Parmaeters: none
	FSU_CMD_TX_FAX,          // Parameters: phone number
	FSU_CMD_TX_DLY,          // Parameters: time to transmit, phone number
	FSU_CMD_BCAST_TIME,      // Parameters: time to broadcast
	FSU_CMD_BCAST_NUMBER,    // Parameters: phone number
	FSU_CMD_BCAST_START,     // Parameters: none
	FSU_CMD_BCAST_CANCEL,    // Parameters: none
	FSU_CMD_REDIAL,          // Parameters: none
	FSU_CMD_MANUAL_RX_FAX,   // Parameters: none
	FSU_CMD_OFF_HOOK,        // Parameters: none
	FSU_CMD_ON_HOOK,         // Parameters: none
	FSU_CMD_DIAL_DIGIT,      // Parameters: dtmf digit
	FSU_CMD_GET_TX_JOBS,     // Parameters: none
	FSU_CMD_GET_TX_INFO,     // Parameters: Tx job reference
	FSU_CMD_CANCEL_TX_JOB,   // Parameters: Tx job reference
	FSU_CMD_PRINT_REPORT,    // Parameters: report type
	FSU_CMD_GET_PRN_JOBS,    // Parameters: none
	FSU_CMD_GET_PRN_INFO,    // Parameters: Print job reference
	FSU_CMD_CANCEL_PRN_JOB,  // Parameters: Print job reference
	FSU_CMD_SET_DATE_TIME,   // Parameters: month, day, year, hours, minutes, dow
	FSU_CMD_SEND_MODEM_CMD,  // Parameters: AT-command string
	FSU_CMD_PAPER_JAM_CLEARED,// Parameters: abort/resume
	FSU_CMD_FLATBED_SCAN,    // Parameters: continue/end
	FSU_CMD_ABORT,           // Parameters: none
	FSU_CMD_STARTUP,         // Parameters: none
	FSU_CMD_ORG_POLL,        // Parameters: phone number to dial and poll
	FSU_CMD_ANS_POLL,        // Parameters: none

// *** NOTE: The following commands are reserved for use by Oasis only ***

	FSU_CMD_INIT_FXCB,       // Parameters: none
	FSU_CMD_SET_FXCB_PARM,   // Parameters: {idx}, {value}
	FSU_CMD_TEST_COMMAND,    // Parameters: {command specific}

/* KB : test for scan manager */
	FSU_CMD_INIT_SCAN_MGR,	 // Not used, Parmaeters: none
	FSU_CMD_CREAT_SCAN_JOB,	 // Parameters: fax_type, scan_res, san_contrast
													 // Responses : one of {FSU_RSP_SCAN_FAIL,FSU_RSP_SCAN_PAGE_FULL,handle}
	FSU_CMD_SCAN_GET_PAGE,	 // Parameters: handle, page no.
													 // Responses : one of {FSU_RSP_SCAN_INVALID_JOB,FSU_RSP_SCAN_WAIT,FSU_RSP_SCAN_DONE,
													 //										 FSU_RSP_SCAN_PAGE_DONE,FSU_RSP_SCAN_WAIT_FOR_MEMORY,
													 //										 FSU_RSP_SCAN_SCANNER_FAILURE,FSU_RSP_SCAN_IN_PROGRESS}
													 //						 followed by 4 bytes if neither FSU_RSP_SCAN_INVALID_JOB nor FSU_RSP_SCAN_WAIT
	FSU_CMD_SCAN_GET_JOBLIST,// Parameters: handle
													 // Responses : toltal # of Jobs, handles in the JOBs
	FSU_CMD_SCAN_GET_JOBINFO,// Parameters: none
													 // Responses : one of {FSU_RSP_SCAN_INVALID_JOB,FSU_SC_RESPONSE_FAIL,FSU_RSP_SCAN_SUCCESS}
													 //						 followed by 6 bytes if FSU_RSP_SCAN_SUCCESS
	FSU_CMD_SCAN_REMOVED_JOB,// Parameters: handle
													 // Responses : one of {FSU_SC_RESPONSE_FAIL,FSU_RSP_SCAN_SUCCESS}
	FSU_CMD_SCAN_FREE_PAGE,	 // Parameters: handle, page no
													 // Responses : one of {FSU_SC_RESPONSE_FAIL,FSU_RSP_SCAN_SUCCESS}
	FSU_CMD_CREATE_HEADER,	 // Parameters: fax_type, scan_res
													 // Responses : one of {FSU_RSP_HDR_FAIL,FSU_RSP_HDR_SUCCESS}
	FSU_CMD_HEADER_STATE,		 //	Parameters: none
													 // Responses : one of {FSU_RSP_HDR_PAGE_DONE,FSU_RSP_HDR_IN_PROGRESS,FSU_RSP_HDR_FAIL}
	FSU_CMD_CREATE_FAXFWD_JOB, // Parameters: none
												     // Responses : one of {FSU_RSP_SCAN_FAIL,handle}
	FSU_CMD_CREATE_FAXFWD_PAGE,// Parameters: handle
													 // Responses : one of {FSU_RSP_SCAN_INVALID_JOB,FSU_SC_RESPONSE_FAIL,FSU_RSP_SCAN_SUCCESS}
	FSU_CMD_CLOSE_FAXFWD_JOB,// Parameters: handle
													 // Responses : one of {FSU_RSP_SCAN_INVALID_JOB,FSU_SC_RESPONSE_FAIL,FSU_RSP_SCAN_SUCCESS}
	FSU_CMD_CREATE_MERGE,    // Parameters: handle
													 // Responses : one of {FSU_RSP_HDR_FAIL,FSU_RSP_HDR_SUCCESS}
	FSU_CMD_GET_MERGED,			 // Parameters: none
													 // Responses : one of {FSU_RSP_HDR_PAGE_DONE,FSU_RSP_HDR_IN_PROGRESS,FSU_RSP_HDR_FAIL}
	FSU_CMD_TEST_FLATBED_SCAN,	// Parameters: handle, command
														// Responses : one of {FSU_RSP_SCAN_FAIL,FSU_RSP_SCAN_SUCCESS}
/* KB : END */

	// P050602, RB: Debug cmds for Print Mgr
	FSU_CMD_CJ,   // create job
	FSU_CMD_CP,   // create (add) page
	FSU_CMD_LJ,   // get job list
	FSU_CMD_IJ,   // get job info
	FSU_CMD_JR    // remove job
};

// -------------------------- //
// Definitions of FSU status: //
// -------------------------- //

// Define limits for string parameter lengths:
#define FSU_STAT_TX_DIAL_STR_MAX    FAX_NUM_SIZE   // Defined in fsu_hdr.h
#define FSU_STAT_PHASE_B_STR_MAX    20             // Limited to 20 by ITU T.30
#define FSU_STAT_CALLER_ID_STR_MAX  40             // Should be less than CID_MAX defined in fsu_hdr.h

// NOTE: Status range from 0x00 to 0x7F.
enum
{
	FSU_STAT_NULL,               // Parameters: none
	FSU_STAT_TX_DIAL,            // Parameters: {phone number (NULL terminated string)}
	FSU_STAT_TX_PHASE_B,         // Parameters: {receiver's CSI (NULL terminated string)}
	FSU_STAT_TX_PHASE_C,         // Parameters: {UINT8 page number}, {UINT8 speed}
	FSU_STAT_TX_PHASE_D,         // Parameters: none
	FSU_STAT_TX_RETRY,           // Parameters: {UINT8 number of attempts left}
	FSU_STAT_RX_RING,            // Parameters: none
	FSU_STAT_RX_PHASE_B,         // Parameters: {sender's TSI (NULL terminated strings)}
	FSU_STAT_RX_PHASE_C,         // Parameters: {UINT8 page number}, {UINT8 speed}
	FSU_STAT_RX_PHASE_D,         // Parameters: none
	FSU_STAT_PRINTING_RPT,       // Parameters: none
	FSU_STAT_BC_CALL_DONE,       // Parameters: none
	FSU_STAT_BC_CALL_ABORT,      // Parameters: none
	FSU_STAT_ABORTING,           // Parameters: none
	FSU_STAT_PRINTER_ERROR,      // Parameters: none
	FSU_STAT_WAIT_FOR_MEMORY,    // Parameters: none
	FSU_STAT_SCANNING_PAGE,      // Parameters: {UINT8 page number}
	FSU_STAT_CALLER_ID,          // Parameters: {Caller I.D. string (NULL terminated string)}
	FSU_STAT_CALLER_ID_NUM,      // Parameters: {Caller I.D. number (NULL terminated string)}
	FSU_STAT_STOP_FAX_DETECT,    // Parameters: none
	FSU_STAT_OFF_HOOK_ALARM,     // Parameters: none
	FSU_STAT_OFF_HOOK_RESTORE,   // Parameters: none
	FSU_STAT_SCANNER_BUSY,       // Parameters: none
	FSU_STAT_SCANNER_READY,      // Parameters: none
	FSU_STAT_SEND_PARTIAL_DOC    // Parameters: none
};

// ------------------------------ //
// Definitions for FSU Responses: //
// ------------------------------ //
// NOTE: Responses range from 0x80 to 0xFF.

// Define limits for string parameter lengths:
#define FSU_RSP_BROADCAST_FAIL_STR_MAX   FAX_NUM_SIZE       // Defined in fsu_hdr.h
#define FSU_RSP_TX_JOBS_PENDING_LEN_MAX  (TX_JOBS + BC_JOBS)// Defined in fsu_hdr.h
#define FSU_RSP_TX_JOB_INFO_STR_MAX      FAX_NUM_SIZE
#define FSU_RSP_PRN_JOBS_PENDING_LEN_MAX MAX_PRINT_JOBS     // Defined in fsu_pr.h
#define FSU_RSP_PRN_JOB_INFO_STR_MAX     FAX_NUM_SIZE

enum
{
	FSU_RSP_INVALID_CMD = 128,// Parameters: none
	FSU_RSP_READY,            // Parameters: none
	FSU_RSP_TX_FAX_DELAY,     // Parameters: none
	FSU_RSP_TX_DONE,          // Parameters: none
	FSU_RSP_TX_FAIL,          // Parameters: {UINT8 error code}
	FSU_RSP_SCAN_ERROR,       // Parameters: none
	FSU_RSP_DOC_TOO_BIG,      // Parameters: none
	FSU_RSP_RX_DONE,          // Parameters: none
	FSU_RSP_RX_FAIL,          // Parameters: {UINT8 error code}
	FSU_RSP_RX_VOICE,         // Parameters: none
	FSU_RSP_RINGING_STOPPED,  // Parameters: none
	FSU_RSP_BROADCAST_DONE,   // Parameters: none
	FSU_RSP_BROADCAST_FAIL,   // Parameters: {UINT8 error code}, {phone number (NULL terminted string)}
	FSU_RSP_MODEM_ERROR,      // Parameters: none
	FSU_RSP_MODEM_TIMEOUT,    // Parameters: none
	FSU_RSP_TX_JOBS_PENDING,  // Parameters: {UINT8[] list of pending Tx jobs}
	FSU_RSP_TX_JOB_INFO,      // Parameters: {UINT16 tx time}, {Phone number (NULL terminated string)}
	FSU_RSP_PRN_JOBS_PENDING, // Parameters: {UINT8[] list of pending print jobs}
	FSU_RSP_PRN_JOB_INFO,     // Parameters: {UINT8 number of pages}, {job I.D. (NULL terminated string)}
	FSU_RSP_PAPER_JAM,        // Parameters: none
	FSU_RSP_FLATBED_SCAN_DONE,// Parameters: none
	FSU_RSP_STARTUP_DONE,     // Parameters: none
	FSU_RSP_DID_NOT_PRINT,    // Parameters: none
	FSU_RSP_TX_QUEUE_FULL,    // Parameters: none
	FSU_RSP_STILL_ON_HOOK,     // Parameters: none

/* KB : test */
	FSU_RSP_SCAN_INIT_DONE,
	FSU_RSP_SCAN_FAIL,
	FSU_RSP_SCAN_SUCCESS,
	FSU_RSP_SCAN_PAGE_FULL,
	FSU_RSP_SCAN_INVALID_JOB,
	FSU_RSP_SCAN_WAIT,
	FSU_RSP_SCAN_DONE,
	FSU_RSP_SCAN_PAGE_DONE,
	FSU_RSP_SCAN_WAIT_FOR_MEMORY,
	FSU_RSP_SCAN_SCANNER_FAILURE,
	FSU_RSP_SCAN_IN_PROGRESS,
	FSU_RSP_DEL_RETRY,
	FSU_RSP_HDR_SUCCESS,
	FSU_RSP_HDR_PAGE_DONE,
	FSU_RSP_HDR_IN_PROGRESS,
	FSU_RSP_HDR_FAIL
/* KB : end */
};


#ifdef __FSU_C__
#if defined(RTOS_THREADX) || defined(RTOS_LINUX)   
    UINT32 fsu_stack[(DEFAULT_STACK_BYTES/4)];
#endif
	    

#endif

#endif  // #ifndef __FXCB_H__

