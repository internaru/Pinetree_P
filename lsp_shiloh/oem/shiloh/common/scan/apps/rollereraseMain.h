#ifndef	ROLLERERASE_MAIN_H_
#define	ROLLERERASE_MAIN_H_

int SDH_eraseRoller_init(void);
int SDH_eraseRoller_isUse(void);

typedef struct erase_roller_MainParam_s {
	unsigned int	scan_job_type;
	unsigned int	pixels_per_row_padded;
	unsigned int	pixels_per_row;
	unsigned int	total_rows;
	unsigned int	BitsPerPixel;
	unsigned int	bytes_per_row;
	unsigned int	scanvar_dpi;
	unsigned int	scanvar_cmode;

	//regarding roller position
	unsigned int	roller_check_current_mode;
	unsigned int	roller_check_current_row_index;
	unsigned int	roller_1st_buffer_save_point;
	unsigned int	roller_2nd_buffer_save_point;
	unsigned int	roller_1st_check_start_row_index;
	unsigned int	roller_2nd_check_start_row_index;
	unsigned int	roller_1st_check_end_row_index;
	unsigned int	roller_2nd_check_end_row_index;
	unsigned int	roller_1st_check_line_nums;
	unsigned int	roller_2nd_check_line_nums;
	unsigned int	roller_1st_check_line_gaps;
	unsigned int	roller_2nd_check_line_gaps;
	unsigned int	roller_start_col_index;
}erase_roller_MainParam_t;

typedef struct scan_flatbed_roller_position_s {
    unsigned int dpi;
	unsigned int roller_position_1st_start;	//REF1_Y
	unsigned int roller_position_1st_width;	//roller_width
	unsigned int roller_position_1st_height;	//roller_height
	unsigned int roller_position_2nd_start;	//REF2_Y
	unsigned int roller_position_2nd_width;	//roller_width
	unsigned int roller_position_2nd_height;	//roller_height
	
	int		roller_height;
	int		roller_width;	
	int		pwl;
	int		pwr;
	int		checklength;
	int		inside;
	int		edgebound;
	int		blackpart;
	int		REF1_X;
	int		REF2_X;
	int		REF1_Y;
	int		REF2_Y;
	int		bgrbound;
	int		paperbound;
	int		roller_inside;
}scan_flatbed_roller_position_t;

typedef enum {
	ROLLER_ERASE_CURRENT_MODE_NORMAL = 0,
	ROLLER_ERASE_CURRENT_MODE_WAIT_1ST_ROLLER = 1,
	ROLLER_ERASE_CURRENT_MODE_START_1ST_ROLLER = 2,
	ROLLER_ERASE_CURRENT_MODE_END_1ST_ROLLER = 3,
	ROLLER_ERASE_CURRENT_MODE_WAIT_2ND_ROLLER = 4,
	ROLLER_ERASE_CURRENT_MODE_START_2ND_ROLLER = 5,
	ROLLER_ERASE_CURRENT_MODE_END_2ND_ROLLER = 6,
}ROLLER_ERASE_MODE;

typedef enum {
	DATA_RECEIVER_NONE_AFTER_ERASE = 0,
	DATA_RECEIVER_SCANMAN_AFTER_ERASE = 1,
	DATA_RECEIVER_MERGEAPP_AFTER_ERASE = 2,
}DATA_RECEIVER;

typedef enum {
	ERASE_JOB_TYPE_NONE=0,
	ERASE_JOB_TYPE_NORMAL=1,
	ERASE_JOB_TYPE_MERGE=2,
} erase_roller_job_type_t;

typedef enum {
	e_HOST_SCAN=0,
	e_HOST_COPY=1,
	e_HOST_FAX=2,
} erase_roller_host_type_t;
#endif
