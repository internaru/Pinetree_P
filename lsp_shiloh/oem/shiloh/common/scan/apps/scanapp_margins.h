#ifndef _SCANAPP_MARGINS_H
#define _SCANAPP_MARGINS_H


#define MARGIN_SP_VALUE_MM			0
#define MARGIN_SP_VALUE_HINCH		0

#define FB_LEFT_MARGIN_HINCH_MIN_VALUE		-7
#define FB_TOP_MARGIN_HINCH_MIN_VALUE		-54
#define ADF_LEFT_MARGIN_HINCH_MAX_VALUE		12

#define MECH_SPEC_JUNIPER_ADF_MAX_WIDTH_HINCHES		850	//Letter Width

scan_err_t scanner_service_get_orig_fb_margins(int dpi, scan_cmode_t mode, struct scan_flatbed_margins *orig_margin);
scan_err_t scanner_service_get_orig_adf_margins(int dpi, scan_cmode_t mode, struct scan_adf_margins *orig_margin);

void scanner_service_write_IQTregistResult_intoDB(int32_t IQT_source, int32_t IQTregistResultMargin_X, int32_t IQTregistResultMargin_Y);
int scanner_service_write_IQTregistResult_FB(int32_t dpi, int32_t IQTregistResultMargin_X, int32_t IQTregistResultMargin_Y);
int scanner_service_write_IQTregistResult_ADF_Front(int32_t dpi, int32_t IQTregistResultMargin_X, int32_t IQTregistResultMargin_Y);
int scanner_service_write_IQTregistResult_ADF_Back(int32_t dpi, int32_t IQTregistResultMargin_X, int32_t IQTregistResultMargin_Y);
int scanner_service_get_margin_deltaValue(int32_t dpi, int32_t mode, int32_t data);

int scanner_service_getDB_marginFB(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom);
int scanner_service_getDB_marginADF(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom);
int scanner_service_getDB_marginADFBack(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom);

int scanner_service_reset_DB(int scan_mode);
int scanner_service_setDB_marginFB(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom);
int scanner_service_setDB_marginADF(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom);
int scanner_service_setDB_marginADFBack(int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom);

int scanner_service_check_margin_value(int mode, int *marginVal_left, int *marginVal_top, int *marginVal_right, int *marginVal_bottom);
int scanner_service_sync_ADF_MarginTable(void);
int scanner_service_sync_ADFBack_MarginTable(void);
int scanner_service_sync_FB_MarginTable(void);
int scanner_service_sync_marginTable(void);

/* Used for IQT Auto Registration with SINDOH's IQT Program */
typedef enum {
	IQT_AUTO_REGISTRATION_FROM_NONE = 0,
	IQT_AUTO_REGISTRATION_FROM_FB = 1,
	IQT_AUTO_REGISTRATION_FROM_ADF_FRONT = 2,
	IQT_AUTO_REGISTRATION_FROM_ADF_BACK = 3,
	IQT_FACTORY_REGISTRATION = 4,
} IQT_AUTO_REGISTRATION_SOURCE;

typedef enum {
	CHANGE_VALUE_FROM_PIXEL_TO_HINCH = 0,
	CHANGE_VALUE_FROM_PIXEL_TO_TMM = 1,
	CHANGE_VALUE_FROM_HINCH_TO_PIXEL = 2,
	CHANGE_VALUE_FROM_HINCH_TO_TMM = 3,
	CHANGE_VALUE_FROM_TMM_TO_HINCH = 4,
	CHANGE_VALUE_FROM_TMM_TO_PIXEL = 5,	
} IQT_MARGIN_VALUE_TYPE;

extern int marginTableSyncFBDone;
extern int marginTableSyncADFDone;
extern int marginTableSyncADFBackDone;

#endif
