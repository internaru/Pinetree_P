/******************************************************************************
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include "cmd_proc_api.h"
#include "lassert.h"
#include "debug.h"
#include "sm_job_api.h"
#include "sm_job_internal_api.h"
#include "sm_job_support_api.h"

//-----------------------------------------------------------------------------
char *smjob_state_to_string( smjob_state_t units )
{
    switch( units )
    {
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_PENDING);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_PENDING_HELD);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_PROCESSING);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_PROCESSING_STOPPED);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_CANCELED);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_ABORTED);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_COMPLETED);
        DEFAULT_CASE_ENUM_RETURN_STRING(smjob_state_t);
    }

    //return "bug"; //UNREACHABLE Code
}

//-----------------------------------------------------------------------------
char *smjob_reason_to_string( smjob_state_reason_t reason )
{
    switch( reason )
    {
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_NONE);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_INCOMING);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_DATA_INSUFFICIENT);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_DOC_ACCESS_ERROR);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_DOC_TRANSFER_ERROR);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_OUTGOING);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_HOLD_UNTIL_SPECIFIED);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_RESOURCES_ARE_NOT_READY);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_PRINTER_STOPPED_PARTLY);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_PRINTER_STOPPED);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_INTERPRETING);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_QUEUED);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_TRANSFORMING);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_QUEUED_FOR_MARKER);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_PRINTING);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_CANCELED_BY_USER);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_CANCELED_BY_OPERATOR);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_CANCELED_AT_DEVICE);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_ABORTED_BY_SYSTEM);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_UNSUPPORTED_COMPRESSION);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_COMPRESSION_ERROR);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_UNSUPPORTED_DOC_FORMAT);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_DOC_FORMAT_ERROR);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_PROCESSING_TO_STOP_POINT);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_SERVICE_OFF_LINE);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_COMPLETED_SUCCESSFULLY);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_COMPLETED_WITH_WARNINGS);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_COMPLETED_WITH_ERRORS);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_DOC_TIMEOUT_ERROR);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_PASSWORD_WAIT);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_JOB_RESTARTABLE);
        CASE_ENUM_RETURN_STRING(SMJOB_STATE_REASON_QUEUE_IN_DEVICE);
        DEFAULT_CASE_ENUM_RETURN_STRING(smjob_reason_t);
    }
    //return "bug"; //UNREACHABLE Code
}

//-----------------------------------------------------------------------------
char *smjob_type_to_string( smjob_type_t job_type )
{
    switch( job_type )
    {
        CASE_ENUM_RETURN_STRING(SMJOB_TYPE_PRINT);
        CASE_ENUM_RETURN_STRING(SMJOB_TYPE_SCAN);
        CASE_ENUM_RETURN_STRING(SMJOB_TYPE_FAXIN);
        CASE_ENUM_RETURN_STRING(SMJOB_TYPE_FAXOUT);
        CASE_ENUM_RETURN_STRING(SMJOB_TYPE_UNSPECIFIED);
        DEFAULT_CASE_ENUM_RETURN_STRING(smjob_type_t);
    }
    //return "bug"; //UNREACHABLE Code
}

//-----------------------------------------------------------------------------
char *smjob_orientation_to_string( smjob_orientation_t orientation )
{
    switch( orientation )
    {
        CASE_ENUM_RETURN_STRING(SMJOB_ORIENTATION_PORTRAIT);
        CASE_ENUM_RETURN_STRING(SMJOB_ORIENTATION_LANDSCAPE);
        CASE_ENUM_RETURN_STRING(SMJOB_ORIENTATION_REVERSE_PORTRAIT);
        CASE_ENUM_RETURN_STRING(SMJOB_ORIENTATION_REVERSE_LANDSCAPE);
        DEFAULT_CASE_ENUM_RETURN_STRING(smjob_orientation_t);
    }

    //return "bug"; //UNREACHABLE Code
}

//-----------------------------------------------------------------------------
char *smjob_quality_to_string( smjob_quality_t quality )
{
    switch( quality )
    {
        CASE_ENUM_RETURN_STRING(SMJOB_QUALITY_DRAFT);
        CASE_ENUM_RETURN_STRING(SMJOB_QUALITY_NORMAL);
        CASE_ENUM_RETURN_STRING(SMJOB_QUALITY_HIGH);
        DEFAULT_CASE_ENUM_RETURN_STRING(smjob_quality_t);
    }
    //return "bug"; //UNREACHABLE Code
}

//-----------------------------------------------------------------------------
char *smjob_resunits_to_string( smjob_resolution_units_t units )
{
    switch( units )
    {
        CASE_ENUM_RETURN_STRING(SMJOB_RES_UNIT_DOTS_PER_INCH);
        CASE_ENUM_RETURN_STRING(SMJOB_RES_UNIT_DOTS_PER_CM);
        DEFAULT_CASE_ENUM_RETURN_STRING(smjob_resolution_units_t);
    }
    //return "bug"; //UNREACHABLE Code
}

//-----------------------------------------------------------------------------
char *smjob_input_source_to_string( smjob_input_source_t units )
{
    switch( units )
    {
        CASE_ENUM_RETURN_STRING(SMJOB_INPUT_SOURCE_ADF);
        CASE_ENUM_RETURN_STRING(SMJOB_INPUT_SOURCE_ADF_DUPLEX);
        CASE_ENUM_RETURN_STRING(SMJOB_INPUT_SOURCE_PLATEN);
        DEFAULT_CASE_ENUM_RETURN_STRING(smjob_input_source_t);
    }
    //return "bug"; //UNREACHABLE Code
}

