/* 
 *
 * ============================================================================
 * Copyright (c) 2006-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 * 
 */ 

/**
 * \file agConnectMgr.h
 *
 * \brief Matches connections with the correct parser and starts the parser
 * running.
 * 
 * A parser must register with the ParserRegister function at startup time.
 *
 * Internal jobs also register parsers and do pipe connects.  
 * Internal pages and other button spawned jobs for instance.
 * This means no jobs run on the system without coming through here,
 * and that no jobs are created except by parsers.
 * 
 * A pipe or connection must first register with the PipeRegister function.
 *
 * A PipeConnect is called on a registered pipe to kick the connection manager
 * into action.  Think of this as a write on a socket kicking the select on the
 * other side.
 *
 * Parsers must send a MSG_END_PARSE upon completion of parsing. This "end" and
 * the PipeConnect's "start" are the 2 inputs into the thread.
 *
 * A pipe (IOReg) is passed to a parser this pipe has read write rewind
 * functions. 
 *
 */

#ifndef CONNECTMGR_H
#define CONNECTMGR_H

#include "agRouter.h"
#include "list.h"
#include "mlist.h"
#include "io_device.h"


/** These are return values from parsers */
#define PARSER_SUCCESS    0   /*!< Returned when job parsed successfully. */
#define PARSER_NOTMINE    -1  /*!< Returned when parser detects the data does
			       	not belong to it at the beginning of the data
			       	stream. */
#define PARSER_ERROR     -2   /*!< Returned when get a parse error while
			       	processing a job, this tells the conmgr it needs
			       	to dump the data. */


typedef enum ParseReadType
{
   PARSEREAD_EXACT = 0,  ///< Read exact amount from io
   PARSEREAD_SOME,       ///< Read until get some data
   PARSEREAD_ANY,        ///< Read any data thats there, return immediatly if none
   PARSEREAD_DELAY       ///< Wait for 100msec and then return with whatever we got.
}PARSE_READ_TYPE;

typedef enum
{
    e_NONE, 
// SDH_ENUM///////////////////
	e_SYSTEMREPORT,
	e_NETWORKREPORT,
	e_SPREPORT,
	e_FAXTXRXREPORT,
	e_FAXTCR,
	e_MEMTXJOBLIST,
	e_ERRLOGREPORT,
	e_T30LOGREPORT,
	e_TCRFORFAXSERVICE,
	e_ADDRESSBOOK,
	e_SPAMFAXLIST, // Spam Fax List
	e_BROADCASTRESULTREPORT, // Fax Broadcast result report
	e_NETIOPAGE,	//Netwok IO print
	e_PIPEIOPAGE,	//Pipe IO print
	e_USBDRIVEPDFPAGE,	// USB pdf file print
	e_USBDRIVEJPGPAGE,	// USB jpg file print
	e_FAX_LONGPAGE,	//FAX print long page
//////////////////////////////
    e_DOCUMENT, 
    e_ZJS_DOCUMENT,
    e_DEMOPAGE,    // REVISIT DAB to be deprecated
    e_CONFIGPAGE,  // REVISIT DAB to be deprecated
    e_MENUPAGE,    // REVISIT DAB to be deprecated
    e_SUPPLYPAGE,  // REVISIT DAB to be deprecated
    e_CALPAGE, 
    e_ALIGNMENTPAGE,
    e_CLEANINGPAGE,
    e_COPYPAGE,
    e_ERRORREPORT, 
    e_SCANCALPAGE,
    e_FAX,
    e_PQPAGE,
    e_WIRELESSTEST,
    e_DIAGNOSTIC,
    e_PHOTOLAYOUT,
    e_DDPAGE,
    e_PRINT_APP_TEST_PAGE
} DOCTYPE;


// forward declaration
#ifndef Observer_t_DEFINED
#define Observer_t_DEFINED
///< event_observer.h 
typedef struct Observer_s Observer_t;
#endif

// forward declaration
#ifndef PrintJobInfo_t_DEFINED
#define PrintJobInfo_t_DEFINED
///< PrintJobInfo.h 
typedef struct PrintJobInfo_s PrintJobInfo_t;
#endif 

///< private internal error flag.
typedef enum
{   
    e_Error = -1,
    e_NoError
} PIPE_STATUS;

// ksjung
typedef enum
{
    e_Cancel = -1,
    e_Accept
} CANCEL_STATUS;

/** State variable used by connection manager.
 */
typedef enum
{
    e_Idle,     ///< waiting for something to happen.
    e_InParser, ///< running in a parser
} PIPE_STATE;

/**
 * This enum is used to specify special handling for the status function
 */
typedef enum
{
    e_NoSpcHandling,
    e_TimedStatus
} STATUS_SPC_HANDLING;

#ifndef PARSEReg_DEFINED
#  define PARSEReg_DEFINED
///< private only used internally
typedef struct PARSEReg_s PARSEReg;
#endif


///< macros to convert old to new function names.
#define ReadConnect(p, d, l, t) pipe_read(p, d, l, t)
#define WriteConnect(p, d, l) pipe_write(p, d, l)
#define RewindData(p, d, l) pipe_read_rewind(p, d, l)
#define SendIOCmd(p, e, v) pipe_ioctl(p, e, v)
#define Cancel(p) pipe_cancel(p) //ksjung


#define PIPE_ID_COOKIE 0x8915fdf1
/**  Connection manager Pipe used by parsers. 
 */
#ifndef IOReg_DEFINED
#  define IOReg_DEFINED
typedef struct IOStruct_s IOReg;
#endif
struct IOStruct_s
{
    ATLISTENTRY registeredList; ///< all registered pipes.  
    ATLISTENTRY waitingList;    ///< active or not in use connection list.


    /// pipe 
    int32_t (*pipe_read) (IOReg *Pipe, void *DataBuffer, uint32_t Length, PARSE_READ_TYPE ReadType); ///< data read
    int32_t (*pipe_write) (IOReg *Pipe , void *DataBuffer, uint32_t Length); ///< data write
    int32_t (*pipe_status) (IOReg *Pipe, void *DataBuffer, uint32_t Length);
    int32_t (*pipe_cmd_read) (IOReg *Pipe , void *DataBuffer, uint32_t Length); ///< unused command reads
    int32_t (*pipe_cmd_write) (IOReg *Pipe , void *DataBuffer, uint32_t Length); ///< command writes; usb status interrupt 
    int32_t (*pipe_cmd_status) (IOReg *Pipe, void *DataBuffer, uint32_t Length);
    int32_t (*pipe_ioctl) (IOReg *Pipe, ioctl_cmd_t Cmd, void *Details);       ///< IOctal interface to pipe
    uint32_t (*pipe_read_rewind) (IOReg *Pipe, void *DataBuffer, uint32_t Length);  ///< rewind read function data only.
    /// private io handles
    int32_t (*pipe_cancel) (IOReg *Pipe); //ksjung Cancel
    

    io_device_handle_t io; ///< private lower level io interface.
    void *DataBuffer; ///< private pointer to the rewind buffer/
    void *BufferPointer; ///< private pointer to data buffer.
    uint32_t  Length;   ///< private amount of data in the rewind buffer.
    uint32_t   RdBufLen; ///< total length of the buffer.
    Channel_type_t PipeType; ///< type of input data on the pipe, scan, fax, print etc.
    PARSEReg *nextParserToRun;  ///< next parser to run or NULL. Private.
    PIPE_STATUS PipeStatus;   ///< Private: internal error flag.
    CANCEL_STATUS CancelStatus;

    PrintJobInfo_t *pjob_info; ///< ustatus and jobtemplate 
    Observer_t *pdevicestatus; ///< pjl device status observer, each pipe can have different settings On change.
    Observer_t *tdevicestatus; ///< pjl device status observer, each pipe can have different settings Timed.

    PIPE_STATE   PipeState;   ///< NB: documentation/usecases?
    PARSEReg *Parser;         ///< The Parser in which we are running.
    uint32_t idCookie;          ///< this constant is used for sanity checking that an object is a pipe == PIPE_ID_COOKIE
};

/** Used to setup/create/register a new parser.
 */
typedef struct 
{

    MODULE_ID ParserID;   ///< The MODULE_ID used to message this parser.
    char *ParseCmpString; ///< The string to look for in data stream when auto language switching.
    uint32_t StringSize;  ///< the size of the ParseCmpString.
    char *ParseString;    ///< String used inXS pjl enter language "string"
    Channel_type_t ParseType; ///< type of input data on the pipe, scan, fax, print etc.
} PARSE_SETUP;
    
/** temporary structure used to Register a pipe.
 */
typedef struct
{
   io_device_handle_t io;
   DOCTYPE DocType; 
   Channel_type_t channel_type;
} PIPE_INFO;


/**
 * \brief External interface functions:
 */
void SysParserInit(void);

/** 
 * \brief Register a parser with the connection manager at startup.
 * @return 0 no failure, assumes we always have memory at system startup.
 */
uint32_t ParserRegister(
    PARSE_SETUP *in ///< construction time only input parameters in a structure.
    );

/** 
 * \brief Register a pipe (I/O channel) with the connection manager.
 * @return a handle that is used for the call to PipeConnect
 */
void *PipeRegister(
    PIPE_INFO *in ///<  construction time only input parameters in a structure.
    );

/**
 * \brief remove a previously registered pipe.
 * This finds a pipe that was registered and removes it from the list.  If the
 * pipe is active then the remove will fail and this will return false.  If 
 * the pipe does not exist at all this will return false.  If the pipe is 
 * on one list and not on the registered list this will assert.
 * \returns bool
 * \retval true The pipe was removed
 * \retval false The pipe was not removed.
 */
bool pipe_remove(IOReg *Pipe);
/** 
 * \brief Signal that data is available on this pipe, must be called after an
 *  e_close.
 *
 *  This will start parsing/construction of the Job. 
 *  The I/O system calls this when data is available for parsing.
 *  Pipe->SendIOCmd(..., e_close, ...) indicates that a PipeConnect must occur
 *  for the pipe to be read from again.
 *
 */
void PipeConnect(
    void *handle ///< handle matches handle returned from PipeRegister.
    );


/** 
 * \brief Parser sends end parse message telling the connection manager that the
 * pipe, should be inspected for more data to possibly be sent to a different
 * parser.
 *
 * I'm done perhaps someone else can make sence of the data stream.
 * MSG_END_PARSE message.
 * 
 */
void ParserToConnectionManagerEndParseMessage(
    IOReg *Pipe, ///< The Pipe the parser just finished with.
    int ParserReturnCode ///< 0 is success, At this time no other return values are acceptable, cleanup on parser thread.
    );

/**
 * \brief Used by PJL "LANGUAGE=PCL" to set the next language to run on the
 * current pipe, to the parser that matches the compare string "PCL".
 *
 * For instance; requires that every parser has a unique A-Z only string naming
 * it (PCL, POSTSCRIPT, ACL).
 *
 * @return  true if a matching parser was found, false if no matching parser was
 * found.
 */
uint32_t 
SetNextParserToRunOnThisPipe(
    IOReg *CurrentPipe,        ///< currentPipe and nextPipe to run are the same. 
    char *ParserCompareString  ///< "PCL" the parser your looking to find.
    );

/** 
 * \brief This and GetPJLLanguageString() is used by pjl to create a list of
 * strings describing each parser pjl ps pcl5c etc.
 */
uint32_t GetNumLanguages(void);

/** 
 * \brief This and GetNumLanguages() is used by pjl to create a list of strings
 * describing each parser pjl ps pcl5c etc.
 */
char *GetPJLLanguageString(uint32_t numLanguages);

#endif 
