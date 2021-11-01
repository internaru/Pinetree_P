#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "api/yajl_parse.h"
#include "json_to_pcl.h"
#include "json_common.h"
#include "tiff2pcl.h"
#include "db_api.h"
#include "spec.h"
#include "barcode_generator.h"
#include "memAPI.h"

#define BUFFER_SIZE 1024
#define DEFAULT_MAIN_TITLE_FONT_SIZE 36
#define DEFAULT_MENU_TITLE_FONT_SIZE 10
#define DEFAULT_SUB_MENU_TITLE_FONT_SIZE 10
#define DEFAULT_ITEM_FONT_SIZE 10
#define FONT_TYPE_DEFAULT FONT_TYPE_UNIVERS

#define MAX_FONT_SIZE 256

enum ControlCode
{
    CONTROL_CODE_BS     = 0x08,
    CONTROL_CODE_HT     = 0x09,
    CONTROL_CODE_LF     = 0x0A,

    CONTROL_CODE_FF     = 0x0C,
    CONTROL_CODE_CR     = 0x0D,
    CONTROL_CODE_SO     = 0x0E,
    CONTROL_CODE_SI     = 0x0F,
    CONTROL_CODE_ESC    = 0x1B,
    CONTROL_CODE_SP     = 0x20
};

enum ParserState
{
    STATE_WAIT_KEY,
    STATE_MAIN_TITLE,
	STATE_MAIN_LOGO,
    STATE_MENU_TITLE,
    STATE_SUB_MENU_TITLE,
    STATE_ITEM_DATA,
    STATE_ITEM_OFFSET_X,
    STATE_POSITION_X,
    STATE_POSITION_Y,
    STATE_NEW_PAGE,
    STATE_ITEM_FONT_SIZE,
    STATE_BASE_X,
	STATE_PAGE_NUMBER,
	STATE_BARCODE_DATA,
	STATE_TONER_DATA,
	STATE_FAXTHUMBNAIL,
	STATE_DATETIME,
};

typedef struct {
    FILE *fp;
    enum ParserState state;
    int x;
    int y;
    int base_x;
    int offset_x;
    int item_font_size;
} Context;

static int yajl_integer(void *ctx, long integerVal)
{
    Context *context = (Context *)ctx;
    switch (context->state)
    {
    case STATE_POSITION_X:
        context->x = (int)integerVal;
        PclSetAbsolutePositionX(context->fp, context->x);
        if (context->x > PCL_PAPER_WIDTH || context->x < 0)
        {
            printf("Error: invalid x position: %d state: %d\n", context->x, context->state);
            return 0;
        }
        break;
    case STATE_POSITION_Y:
        context->y = (int)integerVal;
        PclSetAbsolutePositionY(context->fp, context->y);
        if (context->y > PCL_PAPER_HEIGHT || context->y < 0)
        {
            printf("Error: invalid y position: %d state: %d\n", context->y, context->state);
            return 0;
        }
        break;
    case STATE_ITEM_FONT_SIZE:
        context->item_font_size = (int)integerVal;
        if (context->item_font_size > MAX_FONT_SIZE || context->item_font_size < 0)
        {
            printf("Error: invalid x position: %d state: %d\n", context->item_font_size, context->state);
            return 0;
        }
        break;
    case STATE_ITEM_OFFSET_X:
        {
            context->offset_x = (int)integerVal;
            int x_position = context->base_x + context->offset_x;
            if (x_position > PCL_PAPER_WIDTH || x_position < 0)
            {
                printf("Error: invalid x position: %d state: %d\n", x_position, context->state);
                return 0;
            }
            PclSetAbsolutePositionX(context->fp, x_position);
        }
        break;
    case STATE_BASE_X:
        {
            context->base_x = (int)integerVal;
        }
        break;
    default:
        printf("Error: invalid integer state %d\n", context->state);
        return 0;
        break;
    }
    return 1;
}

static int yajl_string(void *ctx, const unsigned char * stringVal,
                            unsigned int stringLen)
{
    Context *context = (Context *)ctx;

    char * str = (char *) malloc(stringLen + 1);
    if ( str == NULL )	return	0;
    str[stringLen] = 0;
    memcpy(str, stringVal, stringLen);

    switch (context->state)
    {
    case STATE_MAIN_TITLE:
        fprintf(context->fp, "%s", str);
        break;
    case STATE_MAIN_LOGO:
        fprintf(context->fp, "%s", str);
        break;
    case STATE_MENU_TITLE:
        fprintf(context->fp, "%s\n", str);
        break;
    case STATE_SUB_MENU_TITLE:
        fprintf(context->fp, "%s\n", str);
        break;
    case STATE_ITEM_DATA:
        fprintf(context->fp, "%s", str);
        break;
	case STATE_PAGE_NUMBER:
        fprintf(context->fp, "%s", str);
		break;
	case STATE_BARCODE_DATA:
		PclBarcode(context->fp, str);
		break;
	case STATE_TONER_DATA:
		PclTonerChart(context->fp, str);
		break;
	case STATE_FAXTHUMBNAIL:
		PclFaxThumbnail(context->fp, str);
		break;	
    case STATE_DATETIME:
        fprintf(context->fp, "%s", str);
        break;
    default:
		printf("Error: invalid string state %d\n", context->state);
		free(str);
		return	0;
        break;
    }

    //printf("string: '%s'\n", str);
    free(str);
    return 1;
}

//if equal return 1 else return 0
int yajl_key_compare(const char *key, const unsigned char * stringVal, unsigned int stringLen)
{
    if (key[stringLen] != '\0') return 0;
    return !strncmp(key, (const char *)stringVal, stringLen);
}

static int yajl_map_key(void *ctx, const unsigned char * stringVal,
                             unsigned int stringLen)
{
    Context *context = (Context *)ctx;
    
    if (yajl_key_compare(KEY_MENU_TITLE, stringVal, stringLen))
    {
        //PclSetAbsolutePositionXY(context->fp, context->x, context->y);
        context->state = STATE_MENU_TITLE;
        PclSetFont(context->fp, DEFAULT_MENU_TITLE_FONT_SIZE, FONT_STYLE_UPRIGHT, FONT_STROKE_WEIGHT_STANDARD, FONT_TYPE_DEFAULT);
        PclSetAbsolutePositionX(context->fp, context->base_x);
        PclSetRelativePositionY(context->fp, 30);
    }
    else if (yajl_key_compare(KEY_SUB_MENU_TITLE, stringVal, stringLen))
    {
        context->state = STATE_SUB_MENU_TITLE;
        PclSetFont(context->fp, DEFAULT_SUB_MENU_TITLE_FONT_SIZE, FONT_STYLE_UPRIGHT, FONT_STROKE_WEIGHT_STANDARD, FONT_TYPE_DEFAULT);
        PclSetAbsolutePositionX(context->fp, context->base_x + 150);
        PclSetRelativePositionY(context->fp, 20);
    }
    else if (yajl_key_compare(KEY_ITEM, stringVal, stringLen))
    {
        context->state = STATE_WAIT_KEY;
        PclSetFont(context->fp, context->item_font_size, FONT_STYLE_UPRIGHT, FONT_STROKE_WEIGHT_STANDARD, FONT_TYPE_DEFAULT);
        PclSetAbsolutePositionX(context->fp, context->base_x + 100);
        PclSetRelativePositionY(context->fp, 15);
    }
    else if (yajl_key_compare(KEY_DATA, stringVal, stringLen))
    {
        context->state = STATE_ITEM_DATA;
    }
    else if (yajl_key_compare(KEY_BARCODE, stringVal, stringLen))
    {
        context->state = STATE_BARCODE_DATA;
    }
	else if (yajl_key_compare(KEY_TONERCHART, stringVal, stringLen))
	{
		context->state = STATE_TONER_DATA;
	}
	else if (yajl_key_compare(KEY_FAXTHUMBNAIL, stringVal, stringLen))
	{
		context->state = STATE_FAXTHUMBNAIL;
	}
    else if (yajl_key_compare(KEY_OFFSET_X, stringVal, stringLen))
    {
        context->state = STATE_ITEM_OFFSET_X;
    }
    else if (yajl_key_compare(KEY_POSITION_X, stringVal, stringLen)) 
    {
        context->state = STATE_POSITION_X;
    }
    else if (yajl_key_compare(KEY_POSITION_Y, stringVal, stringLen))
    {
        context->state = STATE_POSITION_Y;
    }
    else if (yajl_key_compare(KEY_MAIN_TITLE, stringVal, stringLen))
    {
        context->state = STATE_MAIN_TITLE;
        PclSetFont(context->fp, DEFAULT_MAIN_TITLE_FONT_SIZE, FONT_STYLE_ITALIC, FONT_STROKE_WEIGHT_BOLD, FONT_TYPE_DEFAULT);
        PclSetAbsolutePositionX(context->fp, 1700);
        PclSetAbsolutePositionY(context->fp, 140);
    }
    else if (yajl_key_compare(KEY_MAIN_LOGO, stringVal, stringLen))
    {
        context->state = STATE_MAIN_LOGO;
        PclSetFont(context->fp, DEFAULT_MAIN_TITLE_FONT_SIZE, FONT_STYLE_UPRIGHT, FONT_STROKE_WEIGHT_BOLD, FONT_TYPE_DEFAULT);
        PclSetAbsolutePositionX(context->fp, 240);
        PclSetAbsolutePositionY(context->fp, 140);
    }
    else if (yajl_key_compare(KEY_OUTLINE, stringVal, stringLen))
    {
        PclDrawOutline(context->fp);
    }
    else if (yajl_key_compare(KEY_NEW_PAGE, stringVal, stringLen))
    {
        PclNewPage(context->fp);
    }
    else if (yajl_key_compare(KEY_ITEM_FONT_SIZE, stringVal, stringLen))
    {
        context->state = STATE_ITEM_FONT_SIZE;
    }
    else if (yajl_key_compare(KEY_BASE_X, stringVal, stringLen))
    {
        context->state = STATE_BASE_X;
    }
	else if (yajl_key_compare(KEY_PAGE_NUMBER, stringVal, stringLen))
	{
		context->state = STATE_PAGE_NUMBER;
        PclSetFont(context->fp, context->item_font_size, FONT_STYLE_UPRIGHT, FONT_STROKE_WEIGHT_STANDARD, FONT_TYPE_DEFAULT);
        PclSetAbsolutePositionX(context->fp, 1500);
        PclSetAbsolutePositionY(context->fp, 3000);
	}
	else if (yajl_key_compare(KEY_LOGO_IMAGE, stringVal, stringLen))
	{
		PclAddLogoImage(context->fp);
	}
	else if (yajl_key_compare(KEY_DATETIME, stringVal, stringLen))
	{
		context->state = STATE_DATETIME;
        PclSetFont(context->fp, context->item_font_size, FONT_STYLE_UPRIGHT, FONT_STROKE_WEIGHT_STANDARD, FONT_TYPE_DEFAULT);
        PclSetAbsolutePositionX(context->fp, 300);
        PclSetAbsolutePositionY(context->fp, 3000);
	}
    else
    {
        printf("Error: invalid key\n");
        fwrite(stringVal, 1, stringLen, stdout);
        return 0;
    }

//  printf("%s: ", __func__);
//  fwrite(stringVal, 1, stringLen, stdout);
//  printf(", state %d\n", context->state);

    return 1;
}

int yajl_end_array(void * ctx) {
    Context *context = (Context *)ctx;

    switch (context->state)
    {
    case STATE_ITEM_DATA:
        fprintf(context->fp, "\n");
        break;
    default:
        break;
    }

    return 1;
}

static yajl_callbacks callbacks = {
    NULL, //null
    NULL, //boolean
    yajl_integer, //integer
    NULL, //double
    NULL, //number
    yajl_string, //string
    NULL, //start_map
    yajl_map_key, //map_key
    NULL, //end_map
    NULL, //start_array
    yajl_end_array //end_array
};

int PclParseJson(FILE *input, void *ctx)
{
    unsigned char fileData[BUFFER_SIZE];
    int retval = 0;
    yajl_status stat;
    yajl_handle hand;
	yajl_parser_config cfg = { 0, 0 };
    int done = 0;
    size_t rd = 0;

    hand = yajl_alloc(&callbacks, &cfg, NULL, ctx);

	while (!done) {
        rd = fread((void *) fileData, 1, BUFFER_SIZE, input);
        
        if (rd == 0) done = 1;

        if (done)
            /* parse any remaining buffered data */
            stat = yajl_parse_complete(hand);
        else
            /* read file data, pass to parser */
            stat = yajl_parse(hand, fileData, rd);
        
        if (stat != yajl_status_insufficient_data &&
            stat != yajl_status_ok)
        {
            unsigned char * str = yajl_get_error(hand, 0, fileData, rd);
            fflush(stdout);
            fprintf(stderr, "%s", (char *) str);
            yajl_free_error(hand, str);
            retval = 1;
            break;
        }
    }

    yajl_free(hand);
    return retval;
}



int PclConvertJsonToPcl(const char *in_name, const char *out_name)
{
	FILE* input = NULL;
	FILE* output = NULL;
    int retval = 0;
    Context context;


    input = fopen(in_name, "r");
	if( input == NULL )
		return -1;
	output = fopen(out_name, "w");
	if( output == NULL )
		return -1;
    
    context.fp = output;
    context.state = STATE_WAIT_KEY;
    context.offset_x = 0;
    context.item_font_size = DEFAULT_ITEM_FONT_SIZE;

    PclStart(output);
    retval = PclParseJson(input, (void *)&context);
    PclEnd(output);

    fclose(input);
	fclose(output);

    return retval;
}

int PclSetFontSpacing(FILE *fp, enum FontSpacing spacing)
{
    fprintf(fp, "%c(s%dP", CONTROL_CODE_ESC, spacing);
    return 0;
}

int PclSetFontPitch(FILE *fp, unsigned int pitch)
{
    fprintf(fp, "%c(s%dH", CONTROL_CODE_ESC, pitch);
    return 0;
}

int PclStart(FILE *fp)
{
	error_type_t db_retval = 0;
	int tray = -1;  // 0 is MPT

	tray = db_get_var_int("CONF_PRNT", "Default_Src_Tray", &db_retval);

	if(db_retval == FAIL)
		printf("DB Read Fail\n");

	fprintf(fp, "%%-12345X");
	fprintf(fp, "@PJL COMMENT SINDOH N702_707 Series PCL5e 12.52\n");
	if(gPaperType == LT_BASE)
		fprintf(fp, "@PJL SET PAPER=LETTER\n");
	else
		fprintf(fp, "@PJL SET PAPER=A4\n");
	fprintf(fp, "@PJL SET ROTATESORT=OFF\n");
	fprintf(fp, "@PJL SET ORIENTATION=PORTRAIT\n");
	fprintf(fp, "@PJL SET RESOLUTION=600\n");
	fprintf(fp, "@PJL SET OUTBIN=UPPER\n");
	fprintf(fp, "@PJL SET DUPLEX=OFF\n");
	fprintf(fp, "@PJL SET HOLD=NORMAL\n");
	fprintf(fp, "@PJL SET FILEBOXNAME=\"\"\n");
	fprintf(fp, "@PJL SET FILEBOXPASSWORD=\"\"\n");
	fprintf(fp, "@PJL SET BRIGHTNESS=\"A:0\"\n");
	fprintf(fp, "@PJL SET CONSTRAST=\"A:2\"\n");
	fprintf(fp, "@PJL SET ECONOMODE=OFF\n");
	fprintf(fp, "@PJL SET USERCODE=\"\"\n");
	fprintf(fp, "@PJL SET AUTHENTICATIONUSERID=\"\"\n");
	fprintf(fp, "@PJL SET AUTHENTICATIONPASSWORD=\"\"\n");

	switch(tray)
	{
#if 0  // Always auto tray!
	case TRAY_1:
	    fprintf(fp, "@PJL SET MEDIASOURCE=MANUALFEED\n");
		break;
	case TRAY_2:
	    fprintf(fp, "@PJL SET MEDIASOURCE=TRAY1\n");
		break;
	case TRAY_3:
	    fprintf(fp, "@PJL SET MEDIASOURCE=TRAY2\n");
		break;
	case TRAY_4:
	    fprintf(fp, "@PJL SET MEDIASOURCE=TRAY3\n");
		break;
#endif
	default:
	    fprintf(fp, "@PJL SET MEDIASOURCE=AUTO\n");
		break;
	}

    fprintf(fp, "@PJL SET QTY=1\n");
    fprintf(fp, "@PJL ENTER LANGUAGE=PCL\n");
    fprintf(fp, "%c%c", CONTROL_CODE_ESC, 'e');
    fprintf(fp, "%c&k2G", CONTROL_CODE_ESC);  //PCL Line Termination: CR=CR LF=CR+LF FF=CR+FF
    fprintf(fp, "%c&l0O", CONTROL_CODE_ESC);  //��Ʈ����Ʈ
	if(gPaperType == LT_BASE)
		fprintf(fp, "%c&l2A", CONTROL_CODE_ESC);
	else
		fprintf(fp, "%c&l26A", CONTROL_CODE_ESC);

	switch(tray)
	{
#if 0  // Always auto tray!
	case TRAY_1:
		fprintf(fp, "%c&l2H", CONTROL_CODE_ESC); // MPT
		break;
	case TRAY_2:
		fprintf(fp, "%c&l1H", CONTROL_CODE_ESC); // Tray1
		break;
	case TRAY_3:
		fprintf(fp, "%c&l4H", CONTROL_CODE_ESC); // Tray2
		break;
	case TRAY_4:
		fprintf(fp, "%c&l5H", CONTROL_CODE_ESC); // Tray3
		break;
#endif
	default:
		fprintf(fp, "%c&l7H", CONTROL_CODE_ESC); // Auto Tray
		break;
	}

    fprintf(fp, "\033*t%dR", 300);//set dpi
    //fprintf(fp, "%c&l10E", CONTROL_CODE_ESC);  //Top margin
    PclSetFontSpacing(fp, FONT_SPACING_PROPORTIONAL);
    return 0;
}

int PclEnd(FILE *fp)
{
    /*
    if (tiff2pcl(fp, "20130218_163428.tif") < 0)
    {
        printf("--jeseo-- tiff2pcl failed\n");
    }
    */
    fprintf(fp, "%c%c", CONTROL_CODE_ESC, 'e');
    fprintf(fp, "%%-12345X@PJL EOJ\n");
    fprintf(fp, "%%-12345X");
    return 0;
}

int PclPlainText(FILE *fp, const char *text)
{
    fprintf(fp, "%s", text);
    return 0;
}

int PclSetFont(FILE *fp, int size, enum FontStyle style, enum FontStrokeWeight weight, enum FontType type)
{
    PclSetFontSize(fp, size);
    PclSetFontStyle(fp, style);
    PclSetFontStrokeWeight(fp, weight);
    PclSetFontType(fp, type);
    return 0;
}

int PclSetFontType(FILE *fp, enum FontType type)
{
    fprintf(fp, "%c(s%dT", CONTROL_CODE_ESC, type);
    return 0;
}

//1 point = 1/72 inch
//1 VMI = 1/48 inch
int PclSetFontSize(FILE *fp, int size)
{
    fprintf(fp, "%c(s%dV", CONTROL_CODE_ESC, size);         //Point
    fprintf(fp, "%c&l%dC", CONTROL_CODE_ESC, 48*size/72);   //VMI: vertical motion index - row height
    return 0;
}

int PclSetFontStrokeWeight(FILE *fp, enum FontStrokeWeight weight)
{
    fprintf(fp, "%c(s%dB", CONTROL_CODE_ESC, weight);
    return 0;
}

int PclSetFontStyle(FILE *fp, enum FontStyle style)
{
    fprintf(fp, "%c(s%dS", CONTROL_CODE_ESC, style);
    return 0;
}

int PclSetAbsolutePositionX(FILE *fp, int x)
{
	int letter_adjust = 0;
	if(gPaperType == LT_BASE)
		letter_adjust = 40;
    fprintf(fp, "%c*p%dX", CONTROL_CODE_ESC, x + letter_adjust);
    return 0;
}

int PclSetAbsolutePositionY(FILE *fp, int y)
{
    fprintf(fp, "%c*p%dY", CONTROL_CODE_ESC, y);
    return 0;
}

int PclSetAbsolutePositionXY(FILE *fp, int x, int y)
{
    PclSetAbsolutePositionX(fp, x);
    PclSetAbsolutePositionY(fp, y);
    return 0;
}

int PclSetRelativePositionX(FILE *fp, int x)
{
	if(x > 0)
		fprintf(fp, "%c*p+%dX", CONTROL_CODE_ESC, x);
	else
		fprintf(fp, "%c*p%dX", CONTROL_CODE_ESC, x);
    return 0;
}

int PclSetRelativePositionY(FILE *fp, int y)
{
	if(y > 0)
		fprintf(fp, "%c*p+%dY", CONTROL_CODE_ESC, y);
	else
		fprintf(fp, "%c*p%dY", CONTROL_CODE_ESC, y);
    return 0;
}

int PclSetRelativePositionXY(FILE *fp, int x, int y)
{
    PclSetRelativePositionX(fp, x);
    PclSetRelativePositionY(fp, y);
    return 0;
}

int PclNewPage(FILE *fp)
{
    fprintf(fp, "%c", CONTROL_CODE_FF);
    return 0;
}

int PclDrawOutline(FILE *fp)
{
	//fprintf(fp, "%c\%\%0BIN;SP1;PW0.8;LA4;PA40,40;PR;PD7800,0,0,10500,-7800,0,0,-10500%c\%\%0A", CONTROL_CODE_ESC, CONTROL_CODE_ESC);
	//fprintf(fp, "%c\%\%0BIN;SP1;PW0.8;LA4;PA155,40;PR;PD7800,0,0,10400,-7800,0,0,-10400%c\%\%0A", CONTROL_CODE_ESC, CONTROL_CODE_ESC);
    //fprintf(fp, "%c\%\%0BIN;SP1;PW0.8;LA4;PA155,40;PR;PD7600,0,0,10100,-7600,0,0,-10100%c\%\%0A", CONTROL_CODE_ESC, CONTROL_CODE_ESC);
	if(gPaperType == LT_BASE)
		fprintf(fp, "%c\%\%0BIN;SP1;PW0.8;PA825,9550;PD;PR6550,0;PU;%c\%\%0A", CONTROL_CODE_ESC, CONTROL_CODE_ESC);
	else
		fprintf(fp, "%c\%\%0BIN;SP1;PW0.8;PA705,10250;PD;PR6550,0;PU;%c\%\%0A", CONTROL_CODE_ESC, CONTROL_CODE_ESC);
    return 0;
}

int PCLSetDPI(FILE *fp, int dpi)
{
	fprintf(fp, "%c*t%dR", CONTROL_CODE_ESC, dpi);
	return 0;
}

#define COMPTYPE_UNCOMPRESS 0  // uncompressed
#define COMPTYPE_RUNLENGTH  1  // run-length encodes
#define COMPTYPE_PACKBITS   2  // packbits (bytewise run-length)
#define COMPTYPE_DELTAROW   3  // delta row compression
#define COMPTYPE_BLOCKDATA  4  // block data
#define COMPTYPE_ADAPTIVE   5  // adaptive compression
#define COMPTYPE_CCITT3_1D  6  // CCITT3 1-D compression
#define COMPTYPE_CCITT3_2D  7  // CCITT3 2-D compression
#define COMPTYPE_CCITT4     8  // CCITT4 compression

int PCLSetCompressType(FILE *fp, int compress_type)
{
	fprintf(fp, "%c*b%dM", CONTROL_CODE_ESC, compress_type);
	return 0;
}

int PCLGraphicPixelSize(FILE *fp, int x, int y)
{
	fprintf(fp, "%c*r%ds%dT", CONTROL_CODE_ESC, x, y);
	return 0;
}

int PCLStartGraphic(FILE *fp)
{
	fprintf(fp, "%c*r1A", CONTROL_CODE_ESC);
	return 0;
}

int PCLEndGraphic(FILE *fp)
{
	fprintf(fp, "%c*rB", CONTROL_CODE_ESC);
	return 0;
}

int PclAddLogoImage(FILE *fp)
{
	FILE *logo_fp = NULL;
	char rd1ByteBuffer = 0;
    char *pBuffer = NULL;
    error_type_t db_retval = FAIL;
    pBuffer = db_get_var("INST_MACHINE", "MachineTLI", &db_retval);

    if(strncmp(pBuffer, "P247", 4) == 0 || strncmp(pBuffer, "M247", 4) == 0) {
	    logo_fp = fopen("/app/KatushaCI.pcl", "r");
    }
    else {
	    logo_fp = fopen("/app/SindohCI.pcl", "r");
    }
    
	if( logo_fp != NULL )
	{
		fseek(logo_fp, 64, SEEK_CUR);
		if(gPaperType == LT_BASE)
			fprintf(fp, "%c*p300x0Y", CONTROL_CODE_ESC);
		else
			fprintf(fp, "%c*p260x0Y", CONTROL_CODE_ESC);
		fprintf(fp, "%c*r3F", CONTROL_CODE_ESC);
		fprintf(fp, "%c*r705s151T", CONTROL_CODE_ESC);
		fprintf(fp, "%c*t300R", CONTROL_CODE_ESC);
//	fprintf(fp, "%c&l0E", CONTROL_CODE_ESC);
		fprintf(fp, "%c*v6W%c%c%c%c%c%c", CONTROL_CODE_ESC, 0, 3, 0, 8, 8, 8);
		fprintf(fp, "%c*r1A", CONTROL_CODE_ESC);
		fprintf(fp, "%c*b0Y", CONTROL_CODE_ESC);
		fprintf(fp, "%c*b3M", CONTROL_CODE_ESC);
		fprintf(fp, "%c*b2380W", CONTROL_CODE_ESC);
		while(fread(&rd1ByteBuffer, 1, 1, logo_fp) > 0)
		{
			fprintf(fp, "%c", rd1ByteBuffer);
		}
		fseek(fp, -2, SEEK_CUR);
		PCLEndGraphic(fp);
		fprintf(fp, "%c&p0C", CONTROL_CODE_ESC);

		fclose(logo_fp);
	}
    MEM_FREE_AND_NULL(pBuffer);
    return 0;
}

int PclBarcode(FILE *fp, char* str)
{
	int i = 0; int j = 0;
	unsigned char* barcodeBuf = 0;
	int barcodeWidth = 0;
	int barcodeHeight = 30;
	barcodeBuf = barcodeGen(str, 2, barcodeHeight, &barcodeWidth);
	if (barcodeWidth % 8 != 0)
		barcodeWidth=((barcodeWidth/8)+1)*8;
	barcodeWidth = barcodeWidth/8;
//	printf("barcode H:%d, W:%d\n", barcodeHeight, barcodeWidth);
	PclSetRelativePositionY(fp, -30);
	PCLGraphicPixelSize(fp, barcodeWidth*8, barcodeHeight);
	PCLSetDPI(fp, 300);
	PCLStartGraphic(fp);
	PCLSetCompressType(fp, COMPTYPE_UNCOMPRESS);
	for(i = 0; i < barcodeHeight; i++)
	{
		fprintf(fp, "%c*b%dW", CONTROL_CODE_ESC, barcodeWidth);
		for(j = 0; j < barcodeWidth; j++)
		{
			fprintf(fp, "%c", barcodeBuf[i*barcodeWidth+j]);
		}
	}
	PCLEndGraphic(fp);
	PclSetRelativePositionY(fp, 30);

	MEM_FREE_AND_NULL(barcodeBuf);

	return 0;
}

#define GRAPH_X_SIZE 	400
#define GRAPH_Y_SIZE	30
#define LINEWIDTH		5
#define AXISHEIGHT		5
#define AXISCOUNT		5
int PclTonerChart(FILE *fp, char* str)
{
	int i = 0; int j = 0;
	unsigned char* graBuf = 0;
	int graphWidth = 0;
	int graphHeight = 30;
	int tnrcnt = 0;
	tnrcnt = atoi(str);
	graBuf = graphGen(GRAPH_X_SIZE, GRAPH_Y_SIZE, LINEWIDTH, AXISHEIGHT, AXISCOUNT, tnrcnt, 100, 70);
	graphWidth = GRAPH_X_SIZE;
	graphHeight = GRAPH_Y_SIZE;
	if (graphWidth % 8 != 0)
		graphWidth=((graphWidth/8)+1)*8;
	graphWidth = graphWidth/8;
	printf("graph H:%d, W:%d\n", graphHeight, graphWidth);
	PclSetRelativePositionY(fp, -30);
	PCLGraphicPixelSize(fp, graphWidth*8, graphHeight);
	PCLSetDPI(fp, 300);
	PCLStartGraphic(fp);
	PCLSetCompressType(fp, COMPTYPE_UNCOMPRESS);
	for(i = 0; i < graphHeight; i++)
	{
		fprintf(fp, "%c*b%dW", CONTROL_CODE_ESC, graphWidth);
		for(j = 0; j < graphWidth; j++)
		{
			fprintf(fp, "%c", graBuf[i*graphWidth+j]);
		}
	}
	PCLEndGraphic(fp);
	PclSetRelativePositionY(fp, 30);

	MEM_FREE_AND_NULL(graBuf);

	return 0;
}

#define THUMBNAIL_X_SIZE	1800
#define THUMBNAIL_Y_SIZE	1800
int PclFaxThumbnail(FILE *fp, char* str)
{
	int i = 0; int j = 0;
	//unsigned char* graBuf = 0;
	int graphWidth = 0;
	int graphHeight = 30;
	int scale = 100;
	scale = atoi(str);
	
	FILE *fpThumbnail=NULL;
	fpThumbnail = fopen("/faxTx.raw", "rb");
	
	if ( fpThumbnail == NULL )
		return 0;
	
	graphWidth = THUMBNAIL_X_SIZE;
	graphHeight = THUMBNAIL_Y_SIZE;
	if (graphWidth % 8 != 0)
		graphWidth=((graphWidth/8)+1)*8;
	graphWidth = graphWidth/8;
	printf("graph H:%d, W:%d\n", graphHeight, graphWidth);
	//PclSetRelativePositionY(fp, -2371);
	PCLGraphicPixelSize(fp, graphWidth*8, graphHeight);
	PCLSetDPI(fp, 300);
	PCLStartGraphic(fp);
	PCLSetCompressType(fp, COMPTYPE_UNCOMPRESS);
	for(i = 0; i < graphHeight; i++)
	{
		fprintf(fp, "%c*b%dW", CONTROL_CODE_ESC, graphWidth);
		for(j = 0; j < graphWidth; j++)
		{
			//fprintf(fp, "%c", graBuf[i*graphWidth+j]);
			fprintf(fp, "%c", fgetc(fpThumbnail));
		}
	}
	PCLEndGraphic(fp);
	//PclSetRelativePositionY(fp, 2371);

	//MEM_FREE_AND_NULL(graBuf);
	fclose(fpThumbnail);

	return 0;
}
