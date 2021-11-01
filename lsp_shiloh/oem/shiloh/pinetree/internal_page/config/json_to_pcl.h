#ifndef JSON_TO_PCL_H
#define JSON_TO_PCL_H

//#define FONT_CG_TIMES "(s1p12v0s0b4101T"
//#define FONT_CORONET "(s1p12v1s0b4116T"
//#define FONT_ANTIQUE_OLIVE "(s1p12v0s0b4168T"
//#define FONT_ALBERTUS_MEDIUM "(s1p12v0s1b4362T"

enum FontType
{
    FONT_TYPE_COURIER       = 4099,
    FONT_TYPE_HELVETICA     = 4100,
    FONT_TYPE_TIMES_ROMAN   = 4101,
    FONT_TYPE_LETTER_GOTHIC = 4102,
    FONT_TYPE_SCRIPT        = 4103,
    FONT_TYPE_PRESTIGE      = 4104,
    FONT_TYPE_UNIVERS       = 4148
};

enum FontStrokeWeight
{
    FONT_STROKE_WEIGHT_MINIMUM  = -7,
    FONT_STROKE_WEIGHT_LIGHT    = -3,
    FONT_STROKE_WEIGHT_STANDARD = 0,
    FONT_STROKE_WEIGHT_BOLD     = 3,
    FONT_STROKE_WEIGHT_MAXIMUM  = 7
};

enum FontSpacing
{
    FONT_SPACING_FIXED,
    FONT_SPACING_PROPORTIONAL
};

enum FontStyle
{
    FONT_STYLE_UPRIGHT              = 0,
    FONT_STYLE_ITALIC               = 1,
    FONT_STYLE_CONDENSED            = 4,
    FONT_STYLE_COMPRESSED_ITALIC    = 5,
    FONT_STYLE_EXPANDED             = 24,
    FONT_STYLE_OUTLINE              = 32,
    FONT_STYLE_INLINE               = 64,
    FONT_STYLE_SHADOWED             = 128,
    FONT_STYLE_OUTLINE_SHADOWED     = 160
};

int PclSetFontSpacing(FILE *fp, enum FontSpacing spacing);
int PclSetFontPitch(FILE *fp, unsigned int pitch);
int PclStart(FILE *fp);
int PclEnd(FILE *fp);
int PclPlainText(FILE *fp, const char *text);
int PclSetFont(FILE *fp, int size, enum FontStyle style, enum FontStrokeWeight weight, enum FontType type);
int PclSetFontType(FILE *fp, enum FontType type);
int PclSetFontSize(FILE *fp, int size);
int PclSetFontStrokeWeight(FILE *fp, enum FontStrokeWeight weight);
int PclSetFontStyle(FILE *fp, enum FontStyle style);
int PclSetAbsolutePositionX(FILE *fp, int x);
int PclSetAbsolutePositionY(FILE *fp, int y);
int PclSetAbsolutePositionXY(FILE *fp, int x, int y);
int PclSetRelativePositionX(FILE *fp, int y);
int PclSetRelativePositionY(FILE *fp, int y);
int PclSetRelativePositionXY(FILE *fp, int x, int y);
int PclNewPage(FILE *fp);
int PclDrawOutline(FILE *fp);
int PCLSetDPI(FILE *fp, int dpi);
int PCLSetCompressType(FILE *fp, int compress_type);
int PCLGraphicPixelSize(FILE *fp, int x, int y);
int PCLStartGraphic(FILE *fp);
int PCLEndGraphic(FILE *fp);
int PclAddLogoImage(FILE *fp);
int PclBarcode(FILE *fp, char* str);
int PclTonerChart(FILE *fp, char* str);
int PclFaxThumbnail(FILE *fp, char* str);
int PclConvertJsonToPcl(const char *in_name, const char *out_name);

#endif
