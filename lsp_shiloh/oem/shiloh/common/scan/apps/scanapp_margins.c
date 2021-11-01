#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "scantypes.h"
#include "scanapp_margins.h"
#include "logger.h"
#include "scanapp_uiscan.h"
#include "scanmech.h"
#include "scanman.h"
#include "db_api.h"

/*
 **********************************************************************************************
 * For SINDOH Log System
 **********************************************************************************************
 */ 
#define DBG_PRFX "SCN APP: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(1)
///Using Method.
//DBG_PRINTF_SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
//DBG_PRINTF_SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)

#define USED_FB_MARGIN_TABLE_NUM	12

static struct scan_flatbed_margins flatbed_orig_margins[] = {
    /* dpi, cmode, left_margin_hinches_x, top_margin_hinches_y, right_margin_hinches_x */
    {  75,  SCAN_CMODE_MONO,   7,  54, 864},
    {  75,  SCAN_CMODE_COLOR,  7,  54, 864},    
    { 100,  SCAN_CMODE_MONO,   7,  54, 864},
    { 100,  SCAN_CMODE_COLOR,  7,  54, 864},    
    { 150,  SCAN_CMODE_MONO,   7,  54, 864},
    { 150,  SCAN_CMODE_COLOR,  7,  54, 864},
    { 200,  SCAN_CMODE_MONO,   7,  54, 864},
    { 200,  SCAN_CMODE_COLOR,  7,  54, 864},
    { 300,  SCAN_CMODE_MONO,   7,  54, 864},
    { 300,  SCAN_CMODE_COLOR,  7,  54, 864},
    { 600,  SCAN_CMODE_MONO,   7,  54, 864},
    { 600,  SCAN_CMODE_COLOR,  7,  54, 864},
    { 1200, SCAN_CMODE_MONO,   7,  54, 864},
    { 1200, SCAN_CMODE_COLOR,  7,  54, 864},
    { 2400, SCAN_CMODE_MONO,   7,  54, 864},
    { 2400, SCAN_CMODE_COLOR,  7,  54, 864},

    /* end of table marker */
    { 0, SCAN_CMODE_MONO, 0, 0 },
};


static struct scan_adf_margins adf_orig_margins[] = {
    /* dpi, cmode,            first_tof, tof, bof, left_margin_hinches, rows_to_tof_backside, rows_to_bof_backside */
    {  .dpi=300,
       .cmode=SCAN_CMODE_MONO,
       .rows_to_first_tof=280,
       .rows_to_tof=247,		// 20 -> 240
       .rows_to_bof=190,		// 30 -> 195
       .left_margin_hinches=0,
       .rows_to_tof_backside=850,
    },
    {  .dpi=300,
       .cmode=SCAN_CMODE_COLOR,
       .rows_to_first_tof=280,
       .rows_to_tof=247,
       .rows_to_bof=190,
       .left_margin_hinches=0,
       .rows_to_tof_backside=850,
    },
    {  .dpi=600,
       .cmode=SCAN_CMODE_MONO,
       .rows_to_first_tof=280,
       .rows_to_tof=495,		// 70 -> 505
       .rows_to_bof=420,		// 300 -> 420
       .left_margin_hinches=0,
       .rows_to_tof_backside=1700,  /* no, really */ 	// 1800 -> 1700
    },
    {  .dpi=600,
       .cmode=SCAN_CMODE_COLOR,
       .rows_to_first_tof=280,
       .rows_to_tof=495,
       .rows_to_bof=420,
       .left_margin_hinches=0,
       .rows_to_tof_backside=1700,
    },

    /* end of table marker */
    { .dpi=0, 
      .cmode=SCAN_CMODE_MONO
    },
}; 

int marginTableSyncFBDone = 0;
int marginTableSyncADFDone = 0;
int marginTableSyncADFBackDone = 0;

int scanner_service_check_margin_value(int mode, int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom) 
{	
	if( mode == IQT_AUTO_REGISTRATION_FROM_FB ) {
		if( *marginVal_left < FB_LEFT_MARGIN_HINCH_MIN_VALUE ) {
			*marginVal_left = FB_LEFT_MARGIN_HINCH_MIN_VALUE;
			DBG_PRINTF_SDH_DEBUG("FB Margin(Left) DB Last Value Truncated to(%d)\n", *marginVal_left);
		}
		if( *marginVal_top < FB_TOP_MARGIN_HINCH_MIN_VALUE ) {
			*marginVal_top = FB_TOP_MARGIN_HINCH_MIN_VALUE;
			DBG_PRINTF_SDH_DEBUG("FB Margin(Top) DB Last Value Truncated  to(%d)\n", *marginVal_top);
		}
	}
	else if( mode == IQT_AUTO_REGISTRATION_FROM_ADF_FRONT ) {
		if( *marginVal_left > ADF_LEFT_MARGIN_HINCH_MAX_VALUE ) {
			*marginVal_left = ADF_LEFT_MARGIN_HINCH_MAX_VALUE;
			DBG_PRINTF_SDH_DEBUG("ADF Margin(Left) DB Last Value Truncated to(%d)\n", *marginVal_left);
		}
	}
	else if( mode == IQT_AUTO_REGISTRATION_FROM_ADF_BACK ) {
		
	}
	
	return 0;
}

/**
 * Function name: scanner_service_write_IQTregistResult_intoDB
 *
 * \brief Main Function of Sindoh's IQT result with DB
 **/

/* Sync SINDOH IQT Result with DB value in machine */
void scanner_service_write_IQTregistResult_intoDB(int32_t IQT_source, int32_t IQTregistResultMargin_X, int32_t IQTregistResultMargin_Y) 
{
	DBG_PRINTF_SDH_DEBUG("===============[ #1 ] Received IQT Auto Registration Result from IQT Program ====================\n");
	DBG_PRINTF_SDH_DEBUG("Margin_X(%d)pixel, (%d)pixel\n", IQTregistResultMargin_X, IQTregistResultMargin_Y);
	if(IQT_source == IQT_AUTO_REGISTRATION_FROM_FB) {
		DBG_PRINTF_SDH_DEBUG("IQT_source is(%d) : IQT_AUTO_REGISTRATION_FROM_FB\n", IQT_source);
		scanner_service_write_IQTregistResult_FB(600, IQTregistResultMargin_X, IQTregistResultMargin_Y);
	}
	else if(IQT_source == IQT_AUTO_REGISTRATION_FROM_ADF_FRONT) {
		DBG_PRINTF_SDH_DEBUG("IQT_source is(%d) : IQT_AUTO_REGISTRATION_FROM_ADF_FRONT\n", IQT_source);
		scanner_service_write_IQTregistResult_ADF_Front(600, IQTregistResultMargin_X, IQTregistResultMargin_Y);
	}
	else if(IQT_source == IQT_AUTO_REGISTRATION_FROM_ADF_BACK) {
		DBG_PRINTF_SDH_DEBUG("IQT_source is(%d):	IQT_AUTO_REGISTRATION_FROM_ADF_BACK\n", IQT_source);
		scanner_service_write_IQTregistResult_ADF_Back(600, IQTregistResultMargin_X, IQTregistResultMargin_Y);
	}
}

int scanner_service_write_IQTregistResult_FB(int32_t dpi, int32_t IQTregistResultMargin_X, int32_t IQTregistResultMargin_Y)
{
	//scan margin Delta Value (Flatbed)	
	int leftDeltaHInchVal = 0;
	int topDeltaHInchVal = 0;
		
	/* chage Pixel value to hinch */
	/// PIXEL -> HINCH (1/100inch)
	leftDeltaHInchVal = scanner_service_get_margin_deltaValue(600, CHANGE_VALUE_FROM_PIXEL_TO_HINCH, IQTregistResultMargin_X);
	topDeltaHInchVal = scanner_service_get_margin_deltaValue(600, CHANGE_VALUE_FROM_PIXEL_TO_HINCH, IQTregistResultMargin_Y);

	/* save to DB */
	scanner_service_check_margin_value(IQT_AUTO_REGISTRATION_FROM_FB, &leftDeltaHInchVal, &topDeltaHInchVal, 0, 0);
	scanner_service_setDB_marginFB(&leftDeltaHInchVal, &topDeltaHInchVal, 0, 0);

	DBG_PRINTF_SDH_DEBUG("============== New Scan FB Margin (DB)=================\n");
	DBG_PRINTF_SDH_DEBUG(" FB LEFT  : %d [0.01 inch]\n", leftDeltaHInchVal);
	DBG_PRINTF_SDH_DEBUG(" FB TOP   : %d [0.01 inch]\n", topDeltaHInchVal);

	return 0;
}

int scanner_service_write_IQTregistResult_ADF_Front(int32_t dpi, int32_t IQTregistResultMargin_X, int32_t IQTregistResultMargin_Y)
{
	//scan margin Delta Value (ADF Front)
	int leftDeltaHInchVal = 0;
	int topDeltaHInchVal = 0;
			
	/* chage Pixel value to hinch */
	/// PIXEL -> HINCH (1/100inch)	
	leftDeltaHInchVal = scanner_service_get_margin_deltaValue(600, CHANGE_VALUE_FROM_PIXEL_TO_HINCH, IQTregistResultMargin_X);
	topDeltaHInchVal = scanner_service_get_margin_deltaValue(600, CHANGE_VALUE_FROM_PIXEL_TO_HINCH, IQTregistResultMargin_Y);
		
	/* save to DB */
	scanner_service_check_margin_value(IQT_AUTO_REGISTRATION_FROM_ADF_FRONT, &leftDeltaHInchVal, &topDeltaHInchVal, 0, 0);
	scanner_service_setDB_marginADF(&leftDeltaHInchVal, &topDeltaHInchVal, 0, 0);

	DBG_PRINTF_SDH_DEBUG("==============[ #3 ] New Scan ADF(Front) Margin (DB)=======================\n");
	DBG_PRINTF_SDH_DEBUG(" ADF LEFT  : %d [0.01 inch]\n", leftDeltaHInchVal);
	DBG_PRINTF_SDH_DEBUG(" ADF TOP   : %d [0.01 inch]\n", topDeltaHInchVal);
	
	return 0;
}

int scanner_service_write_IQTregistResult_ADF_Back(int32_t dpi, int32_t IQTregistResultMargin_X, int32_t IQTregistResultMargin_Y)
{
	//scan margin Delta Value (ADF Back)
	int leftDeltaHInchVal = 0;
	int topDeltaHInchVal = 0;
			
	/* chage Pixel value to hinch */
	/// PIXEL -> HINCH (1/100inch)
	leftDeltaHInchVal = scanner_service_get_margin_deltaValue(600, CHANGE_VALUE_FROM_PIXEL_TO_HINCH, IQTregistResultMargin_X);
	topDeltaHInchVal = scanner_service_get_margin_deltaValue(600, CHANGE_VALUE_FROM_PIXEL_TO_HINCH, IQTregistResultMargin_Y);	

	/* save to DB */
	scanner_service_check_margin_value(IQT_AUTO_REGISTRATION_FROM_ADF_BACK, &leftDeltaHInchVal, &topDeltaHInchVal, 0, 0);
	scanner_service_setDB_marginADFBack(&leftDeltaHInchVal, &topDeltaHInchVal, 0, 0);

	DBG_PRINTF_SDH_DEBUG("==============[ #3 ] New Scan ADF(Back) Margin (DB)=======================\n");
	DBG_PRINTF_SDH_DEBUG(" ADF LEFT  : %d [0.01 inch]\n", leftDeltaHInchVal);
	DBG_PRINTF_SDH_DEBUG(" ADF TOP   : %d [0.01 inch]\n", topDeltaHInchVal);
	
	return 0;
}

int scanner_service_sync_FB_MarginTable()
{
	//scan margin (Flatbed)
	int tempScanMarLeft = 0;
	int tempScanMarRight = 0;
	int tempScanMarTop = 0;
	int tempScanMarBottom = 0;
	int idx = 0;
	
	/* get DB Value (1/100 inch) */
	scanner_service_getDB_marginFB(&tempScanMarLeft, &tempScanMarTop, &tempScanMarRight, &tempScanMarBottom);

 	DBG_PRINTF_SDH_DEBUG("==============[ #4 ] New Scan FB Margin (Kernel Table)====\n");
 	
 	for(idx=0; idx<USED_FB_MARGIN_TABLE_NUM; idx++) {
		int dpi;
		int mode;    
		struct scan_flatbed_margins oirg_fb_margins;
		struct scan_flatbed_margins new_fb_margins;
		int leftDelta = 0;
		int topDelta = 0;		
		scan_err_t scerr = SCANERR_NONE;

		if(idx == 0) { dpi = 75; mode = SCAN_CMODE_MONO; }
		else if(idx == 1) { dpi = 75; mode = SCAN_CMODE_COLOR; }
		else if(idx == 2) { dpi = 100; mode = SCAN_CMODE_MONO; }
		else if(idx == 3) { dpi = 100; mode = SCAN_CMODE_COLOR; }
		else if(idx == 4) { dpi = 150; mode = SCAN_CMODE_MONO; }
		else if(idx == 5) { dpi = 150; mode = SCAN_CMODE_COLOR; }
		else if(idx == 6) { dpi = 200; mode = SCAN_CMODE_MONO; }
		else if(idx == 7) { dpi = 200; mode = SCAN_CMODE_COLOR; }	
		else if(idx == 8) { dpi = 300; mode = SCAN_CMODE_MONO; }
		else if(idx == 9) { dpi = 300; mode = SCAN_CMODE_COLOR; }
		else if(idx == 10) { dpi = 600; mode = SCAN_CMODE_MONO; }
		else if(idx == 11) { dpi = 600; mode = SCAN_CMODE_COLOR; }
		else break;
		
		/* get original(CONST)s Margin Table */
		memset( &oirg_fb_margins, 0, sizeof(struct scan_flatbed_margins) );
		scerr = scanner_service_get_orig_fb_margins(dpi, mode, &oirg_fb_margins);
		if( scerr != SCANERR_NONE ) {
			DBG_PRINTF_SDH_DEBUG("Can't get FB Margin[%d]. ERR: %d\n", idx, scerr);
			return -1;
		}					
		
		/* set modified Margin Table */
		leftDelta = tempScanMarLeft;
		topDelta = tempScanMarTop;
				
		memset( &new_fb_margins, 0, sizeof(struct scan_flatbed_margins) );
		new_fb_margins.dpi = dpi;
		new_fb_margins.cmode = mode;
		new_fb_margins.left_margin_hinches_x = MAX(0, oirg_fb_margins.left_margin_hinches_x + leftDelta);
		new_fb_margins.top_margin_hinches_y = MAX(0, oirg_fb_margins.top_margin_hinches_y + topDelta);
		new_fb_margins.right_margin_hinches_x = oirg_fb_margins.right_margin_hinches_x;
		new_fb_margins.bottom_margin_hinches_y = oirg_fb_margins.bottom_margin_hinches_y;
		
		scerr = scanmech_debug_set_fb_margins(&new_fb_margins);
		if( scerr != SCANERR_NONE ) {
			DBG_PRINTF_SDH_DEBUG("Can't set FB Margin[%d]. ERR: %d\n", idx, scerr);
			return -1;

		}
		DBG_PRINTF_SDH_DEBUG("FB Margin Sync => DPI[%d], Mode[%d] : Left(%d), Top(%d)\n", dpi, mode, new_fb_margins.left_margin_hinches_x, new_fb_margins.top_margin_hinches_y);
	}
	
	return 0;
}

int scanner_service_sync_ADF_MarginTable()
{
	int tempScanMarLeft = 0;
	int tempScanMarTop = 0;
	int tempScanMarRight = 0;
	int tempScanMarBottom = 0;

	int tempScanMarLeftBackSide = 0;
	int tempScanMarTopBackSide = 0;
	int tempScanMarRightBackSide = 0;
	int tempScanMarBottomBackSide = 0;	
	
	int idx = 0;
	
	/* get DB Value (1/100 inch) : ADF (Front) */
	scanner_service_getDB_marginADF(&tempScanMarLeft, &tempScanMarTop, &tempScanMarRight, &tempScanMarBottom);
	/// HINCH -> PIXEL
	tempScanMarTop = scanner_service_get_margin_deltaValue(600, CHANGE_VALUE_FROM_HINCH_TO_PIXEL, tempScanMarTop);
	
	/* get DB Value (1/100 inch) : ADF (Back) */	
	scanner_service_getDB_marginADFBack(&tempScanMarLeftBackSide, &tempScanMarTopBackSide, &tempScanMarRightBackSide, &tempScanMarBottomBackSide);
	/// HINCH -> PIXEL
	tempScanMarTopBackSide = scanner_service_get_margin_deltaValue(600, CHANGE_VALUE_FROM_HINCH_TO_PIXEL, tempScanMarTopBackSide);	
	
	    
	DBG_PRINTF_SDH_DEBUG("==============[ #4 ] New Scan ADF Margin (Kernel Table)====\n");
	
 	for(idx=0; idx<4; idx++) {
		int dpi;
		int mode;    
		struct scan_adf_margins oirg_adf_margins;
		struct scan_adf_margins new_adf_margins;
		int leftDelta = 0;
		int topDelta = 0;
		int topBackDelta = 0;
		scan_err_t scerr = SCANERR_NONE;
    		
		if(idx == 0) { dpi = 300; mode = SCAN_CMODE_MONO; }
		else if(idx == 1) { dpi = 300; mode = SCAN_CMODE_COLOR; }
		else if(idx == 2) { dpi = 600; mode = SCAN_CMODE_MONO; }
		else if(idx == 3) { dpi = 600; mode = SCAN_CMODE_COLOR; }
		else break;
		
		/* get original(CONST) Margin Table */
		memset( &oirg_adf_margins, 0, sizeof(struct scan_adf_margins) );
		scerr = scanner_service_get_orig_adf_margins(dpi, mode, &oirg_adf_margins);
		if( scerr != SCANERR_NONE ) {
			DBG_PRINTF_SDH_DEBUG("Can't get ADF Margin[%d]. ERR: %d\n", idx, scerr);
			return -1;
		}
		
		/* set modified Margin Table */
		leftDelta = tempScanMarLeft;
		if( dpi != 600 ) {
			topDelta = tempScanMarTop/(600/dpi);
			topBackDelta = tempScanMarTopBackSide/(600/dpi);
		}
		else {
			topDelta = tempScanMarTop;
			topBackDelta = tempScanMarTopBackSide;
		}
				
		memset( &new_adf_margins, 0, sizeof(struct scan_adf_margins) );
		new_adf_margins.dpi 						= dpi;
		new_adf_margins.cmode					= mode;
		new_adf_margins.rows_to_first_tof		= oirg_adf_margins.rows_to_first_tof;
		new_adf_margins.rows_to_tof 			= oirg_adf_margins.rows_to_tof + topDelta;	
		new_adf_margins.rows_to_bof				= oirg_adf_margins.rows_to_bof;
		new_adf_margins.left_margin_hinches 	= MAX(0, oirg_adf_margins.left_margin_hinches - leftDelta); // have to negative of DB
		new_adf_margins.rows_to_tof_backside	= oirg_adf_margins.rows_to_tof_backside + topBackDelta;
		
		scerr = scanmech_debug_set_adf_margins(&new_adf_margins);
		if( scerr!=SCANERR_NONE ) {
			DBG_PRINTF_SDH_DEBUG("Can't set ADF Margin[%d]. ERR: %d\n", idx, scerr);
			return -1;
		}
		DBG_PRINTF_SDH_DEBUG("ADF Margin Sync => DPI[%d], Mode[%d] : Left(%d), F-Top(%d), B-Top(%d)\n", 
								dpi, mode, new_adf_margins.left_margin_hinches, new_adf_margins.rows_to_tof, new_adf_margins.rows_to_bof);
	}
	
	return 0;
}

int scanner_service_sync_marginTable()
{
	scanner_service_sync_FB_MarginTable();
	scanner_service_sync_ADF_MarginTable();
	
	return 0;
}

int scanner_service_get_margin_deltaValue(int32_t dpi, int32_t mode, int32_t data)
{
	/* 
	 * 600dpi : it means 600dot in 1 inch, 6dot in 0.01inch
	 * The minimum value of SINDOH's margins value is 0.01inch 
	 * The DB Value is controlled in 0.01inch, So the base value is '6'(pixel)
	 */
	 
	int32_t retValue = 0;
	int32_t deltaValue = 0;
	int32_t determinePositive = 1;
	int32_t minimumStepPixel = 0;
	
	minimumStepPixel = dpi / 100; //In Sindoh System, IQT Program is set into 600dpi, So minimum step Pixel is '6'
	
	/// Determine positive or negative
	if(data < 0) {
		determinePositive = -1;	//Delta value of IQT Result is negative
	}
	else {
		determinePositive = 1;	//Delta value of IQT Result is positive	
	}
	
	/// Verification delta value, make positive
	deltaValue = abs(data);
	
	/// Calculate the value
	if( mode == CHANGE_VALUE_FROM_PIXEL_TO_HINCH ) {
		if( deltaValue < minimumStepPixel ) {
			retValue = 0;
		}
		else {
			retValue = ( deltaValue / minimumStepPixel ) * determinePositive;
		}
	}
	else if( mode == CHANGE_VALUE_FROM_HINCH_TO_PIXEL) {
		retValue = ( deltaValue * dpi ) / 100;
		retValue *= determinePositive;
	}
	
	return retValue;
}

int scanner_service_reset_DB(int scan_mode)
{
	int leftVal = 0;
	int topVal = 0;
	int rightVal = 0;
	int bottomVal = 0;

	if( scan_mode == IQT_AUTO_REGISTRATION_FROM_FB ) {
		scanner_service_setDB_marginFB( &leftVal, &topVal, &rightVal, &bottomVal );
	}
	else if( scan_mode == IQT_AUTO_REGISTRATION_FROM_ADF_FRONT ) {
		scanner_service_setDB_marginADF( &leftVal, &topVal, &rightVal, &bottomVal );
	}
	else if( scan_mode == IQT_AUTO_REGISTRATION_FROM_ADF_BACK ) {
		scanner_service_setDB_marginADFBack( &leftVal, &topVal, &rightVal, &bottomVal );
	}
	
	return 0;
}

int scanner_service_getDB_marginFB(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom)
{
	error_type_t db_retval = FAIL;
	*marginVal_left = db_get_var_int("CONF_SCANSUB", "FBLeftMargin", &db_retval);
	*marginVal_top = db_get_var_int("CONF_SCANSUB", "FBTopMargin", &db_retval);
//	*marginVal_right = db_get_var_int("CONF_SCANSUB", "FBRightMargin", &db_retval);
//	*marginVal_bottom = db_get_var_int("CONF_SCANSUB", "FBBottomMargin", &db_retval);
    if( db_retval == FAIL ) { 
		return -1;	
	}
	return 0;
}

int scanner_service_getDB_marginADF(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom)
{
	error_type_t db_retval = FAIL;
	*marginVal_left = db_get_var_int("CONF_SCANSUB", "ADFLeftMargin", &db_retval);
	*marginVal_top = db_get_var_int("CONF_SCANSUB", "ADFTopMargin", &db_retval);
//	*marginVal_right = db_get_var_int("CONF_SCANSUB", "ADFRightMargin", &db_retval);
//	*marginVal_bottom = db_get_var_int("CONF_SCANSUB", "ADFBottomMargin", &db_retval);
	if( db_retval == FAIL ) { 
		return -1;	
	}
	return 0;	
}

int scanner_service_getDB_marginADFBack(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom)
{
	error_type_t db_retval = FAIL;
	*marginVal_left = db_get_var_int("CONF_SCANSUB", "ADFDuplexLeftMargin", &db_retval);
	*marginVal_top = db_get_var_int("CONF_SCANSUB", "ADFDuplexTopMargin", &db_retval);
//	*marginVal_right = db_get_var_int("CONF_SCANSUB", "ADFDuplexRightMargin", &db_retval);
//	*marginVal_bottom = db_get_var_int("CONF_SCANSUB", "ADFDuplexBottomMargin", &db_retval);
	if( db_retval == FAIL ) { 
		return -1;	
	}
	return 0;	
}

int scanner_service_setDB_marginFB(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom)
{
	db_set_var_int("CONF_SCANSUB", "FBLeftMargin", *marginVal_left);
	db_set_var_int("CONF_SCANSUB", "FBTopMargin", *marginVal_top);
//	db_set_var_int("CONF_SCANSUB", "FBRightMargin", *marginVal_right);
//	db_set_var_int("CONF_SCANSUB", "FBBottomMargin", *marginVal_bottom);

	return 0;
}

int scanner_service_setDB_marginADF(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom)
{
	db_set_var_int("CONF_SCANSUB", "ADFLeftMargin", *marginVal_left);
	db_set_var_int("CONF_SCANSUB", "ADFTopMargin", *marginVal_top);
//	db_set_var_int("CONF_SCANSUB", "ADFRightMargin", *marginVal_right);
//	db_set_var_int("CONF_SCANSUB", "ADFBottomMargin", *marginVal_bottom);

	return 0;	
}

int scanner_service_setDB_marginADFBack(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom)
{
	db_set_var_int("CONF_SCANSUB", "ADFDuplexLeftMargin", *marginVal_left);
	db_set_var_int("CONF_SCANSUB", "ADFDuplexTopMargin", *marginVal_top);
//	db_set_var_int("CONF_SCANSUB", "ADFDuplexRightMargin", *marginVal_right);
//	db_set_var_int("CONF_SCANSUB", "ADFDuplexBottomMargin", *marginVal_bottom);	
	
	return 0;	
}

scan_err_t scanner_service_get_orig_fb_margins(int dpi, scan_cmode_t mode, struct scan_flatbed_margins *orig_margin)
{
	int i;
	for( i=0; ; i++ ) {
		if( flatbed_orig_margins[i].dpi == 0 ) {
			DBG_PRINTF_SDH_DEBUG("Fail Get original FB margin [%d]dpi\n", dpi);
			return SCANERR_NO_ENTRY;
		}
		
		if( flatbed_orig_margins[i].dpi == dpi && flatbed_orig_margins[i].cmode == mode ) {
			orig_margin->left_margin_hinches_x = flatbed_orig_margins[i].left_margin_hinches_x;
			orig_margin->top_margin_hinches_y = flatbed_orig_margins[i].top_margin_hinches_y;
			orig_margin->right_margin_hinches_x = flatbed_orig_margins[i].right_margin_hinches_x;
			orig_margin->bottom_margin_hinches_y = flatbed_orig_margins[i].bottom_margin_hinches_y;
			//DBG_PRINTF_SDH_DEBUG("Success Get original FB margin [%d]dpi\n", dpi);
			return SCANERR_NONE;
		}
	}
	return SCANERR_NONE;
}

scan_err_t scanner_service_get_orig_adf_margins(int dpi, scan_cmode_t mode, struct scan_adf_margins *orig_margin)
{
	int i;
	for( i=0; ; i++ ) {
		if( adf_orig_margins[i].dpi == 0 ) {
			DBG_PRINTF_SDH_DEBUG("Fail Get original ADF margin [%d]dpi\n", dpi);
			return SCANERR_NO_ENTRY;
		}
		
		if( adf_orig_margins[i].dpi == dpi && flatbed_orig_margins[i].cmode == mode ) {
			orig_margin->rows_to_first_tof = adf_orig_margins[i].rows_to_first_tof;
			orig_margin->rows_to_tof = adf_orig_margins[i].rows_to_tof;
			orig_margin->rows_to_bof = adf_orig_margins[i].rows_to_bof;
			orig_margin->left_margin_hinches = adf_orig_margins[i].left_margin_hinches;
			orig_margin->rows_to_tof_backside = adf_orig_margins[i].rows_to_tof_backside;
			orig_margin->rows_to_bof_backside = adf_orig_margins[i].rows_to_bof_backside;
			//DBG_PRINTF_SDH_DEBUG("Success Get original ADF margin [%d]dpi\n", dpi);			
			return SCANERR_NONE;
		}
	}
	return SCANERR_NONE;
}
