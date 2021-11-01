/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __HTML_H__
#define __HTML_H__
#include "ATypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "oid_api.h"

#define SSI_IDc    "IDc"
#define SSI_IDr    "IDr"
#define SSI_IDs    "IDs"
#define SSI_IDe    "IDe"
#define SSI_ID     "ID"
#define SSI_IF     "IF"
#define SSI_ELSEIF "ELSEIF"
#define SSI_ELSE   "ELSE"
#define SSI_ENDIF  "ENDIF"
#define SSI_INCLUDE "INCLUDE"

#define SSI_EQ     "EQ"
#define SSI_NEQ    "NEQ"
#define SSI_GT     "GT"
#define SSI_LT     "LT"
#define SSI_GTE    "GTE"
#define SSI_LTE    "LTE"
#define SSI_ERR    "ERR"
#define SSI_AND    "AND"
#define SSI_NAND   "NAND"
#define SSI_LOCAL  "LOCAL"

#define DUMMY_PASSWORD  "$%,.:;##@!*"

//indices used to reference tag args
#define INDEX_CMD          0    //cmd is always first
#define INDEX_COND_OPER    1
#define INDEX_COND_OID     2
#define INDEX_COND_VAL     3
#define INDEX_COND_OUTPUT  4
#define INDEX_IDx_OID      1


#define SSIOPTION_H  0x00000001   //start of hidden, save value as initial value
#define SSIOPTION_O  0x00000002   //compare val is an oid
#define SSIOPTION_S  0x00000004   //compare val is a str enum
#define SSIOPTION_P  0x00000008   //variable is a password
#define SSIOPTION_B  0x00000010   //variable is a bitmask OR value into current hidden value
#define SSIOPTION_D  0x00000020   //end of hidden, save the current value

#define MAX_NESTS 6
#define HTTP_MAX_SSI_TAG 256

/* TODO determine what value this needs to be for the XML/HTML files */
#define EWS_HTTP_GOOD_STATUS 200
#define EWS_DEVICE_FUNCTIONS_COUNT 1
#define NET_DEFAULTS_CONNECTION_MAX 8
#define NET_DEFAULTS_IPV4_ALLOW_DHCP_CONFIG 1
#define NET_DEFAULTS_IPV4_ALLOW_BOOTP_CONFIG 1
#define NET_DEFAULTS_IPV4_ALLOW_AUTOIP_CONFIG 1
#define NET_DEFAULTS_IPV4_SUBNET_MASK "255.255.255.0"
#define NET_DEFAULTS_BONJOUR_ENABLED 1


typedef enum
{
    POPT_CMD = 0,
    POPT_ARGS,
    POPT_ALL
}PARSEOPTION;


typedef enum
{
    SSITAG_IDc = 0,
    SSITAG_IDr,
    SSITAG_IDs,
    SSITAG_IDe,
    SSITAG_ID,
    SSITAG_IF,
    SSITAG_ELSEIF,
    SSITAG_ELSE,
    SSITAG_ENDIF,
    SSITAG_INCLUDE,
    NUM_TAGS,
    SSITAG_INVALID,
}SSITAGTYPE;

typedef enum
{
    OPER_EQ = 0,
    OPER_NE,
    OPER_GT,
    OPER_LT,
    OPER_GTE,
    OPER_LTE,
    OPER_ERR,    //true if errors on previous post
    OPER_AND,    //binary AND operator, used to see if bit is set
    OPER_NAND,   //binary NAND operator, used to see if bit is clear
    OPER_LOCAL,  //true if local host connection false otherwise
    NUM_OPERS,
    OPER_NONE,
}SSIOPER;

#define MAX_ERRORS 25
#define MAX_HIDDEN 3
typedef struct
{
    oid_t       oid;
    int32_t     index;
    uint32_t    val;
} HIDDEN;

typedef struct
{
    BOOL condRes[MAX_NESTS];           // result of conditional, for disabling and enabling output 
    BOOL condOutEnabled[MAX_NESTS];    //true of output enabled in the conditional
    BOOL newpage;                      // set to true everytime we start a new page
    uint8_t creationType;                //HTTP_GET or HTTP_POST, which operation created connection
    uint8_t opCount;               //num operations (Gets or posts) on this connection
    uint32_t oidErrList[MAX_ERRORS];     //list of oids that failed on post
    uint32_t oidIndexErrList[MAX_ERRORS];   //list of oid indices that failed on post
    uint32_t oidErrCount;                //number of oids that failded in post
    HIDDEN hiddenOids[MAX_HIDDEN];     //hidden oids being processed in post
    uint32_t hiddenCount;                //number of hidden oids being processed
    uint32_t nestCount;                   // how many nested IF statements we have found
    char tagBuff[HTTP_MAX_SSI_TAG];
}HTMLSTATE;

typedef struct
{
    SSITAGTYPE tagType; // what kindof agtag
    uint8_t oidType;    // the data type of oid1
    oid_t oid1;         // oid index or string mgr enum for required oid
    oid_t oid2;         // oid index or string mgr enum for optional oid
    uint32_t oid1Index; // oid sub index for oids that are based off enums...
    uint32_t oid2Index; // oid sub index for oids that are based off enums...
    uint32_t options;     // options attached to ID
    uint32_t numArgs;     // number of args including the tag command in this tag
    SSIOPER oper;       // operator for conditionals
    char* val;          // compare value string for conditionals
    char* output;       // output string for IDc
}SSITAG;

#define NUM_ARGS 6
typedef struct
{
    uint8_t numArgs;
    char* arg[NUM_ARGS];
}SSIARGS;


typedef struct
{
    char* mapStr;
    uint32_t mapType;
    uint32_t mapStrLen;
}HTMLMAP;

#define HTTP_OUTPUT_ENABLE          1
#define HTTP_OUTPUT_DISABLE         0

#ifdef __cplusplus
}
#endif

#endif
