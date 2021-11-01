/***********************************************************
* (c) Copyright 2008 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

/******************************************************************************
 *
 * Description:
 *
 *****************************************************************************/


#ifndef _FWVERSION_H
#define _FWVERSION_H

// BEWARE!  The date code string MUST remain 8 characters long!

extern char FIRMWARE_VERSION_STRING[];   // year,month,day

extern char BUILD_INFO_STRING[];

extern char GIT_COMMIT_INFO_STRING[];

extern char GIT_BRANCH_INFO_STRING[];

char * Get_Firmware_Version_String(void);
char * Get_Build_Info_String(void);
char * Get_GIT_Commit_Info_String(void);
char * Get_GIT_Branch_Info_String(void);
char * Get_GIT_Tag_String(void);


/**
 * \brief Initialize the fw_version module/utility
 *
 */
void fw_version_init( void );


/**
 * \brief Get the Module Version Information
 *
 */
char * get_version_report(void);
char * get_version_engine(void);
char * get_version_PDLParser(void);
char * get_version_PJLParser(void);
char * get_version_GDIParser(void);
char * get_version_SindohParser(void);
char * get_version_FaxApp(void);
char * get_version_ScanSubsystem(void);
char * get_version_UI(void);
char * get_version_UICC(void);
char * get_version_PSGUI(void);
char * get_version_PSBrowser(void);

#endif // _FWVERSION_H

