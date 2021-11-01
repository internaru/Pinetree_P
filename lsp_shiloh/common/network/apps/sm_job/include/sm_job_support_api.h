/******************************************************************************
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/** \file sm_job_cmd_api.h
 *
 * \brief These functions are primarily for smjob support
 *
 */

/** \brief The following are used by sm_job_cmd for ease of debug */
char *smjob_state_to_string( smjob_state_t units );
char *smjob_reason_to_string( smjob_state_reason_t reason );
char *smjob_type_to_string( smjob_type_t job_type );
char *smjob_comp_to_string( smjob_compression_t compression );
char *smjob_format_to_string( smjob_format_t format );
char *smjob_orientation_to_string( smjob_orientation_t orientation );
char *smjob_sides_to_string( smjob_sides_t sides );
char *smjob_quality_to_string( smjob_quality_t quality );
char *smjob_resunits_to_string( smjob_resolution_units_t units );
char *smjob_input_source_to_string( smjob_input_source_t units );

