#ifndef JSON_COMMON_H
#define JSON_COMMON_H

#define MM_PER_INCH         25.4f
#define PCL_PAPER_WIDTH     (4600/2)
#define PCL_PAPER_HEIGHT    (6800/2)
#define PCL_COLUMN_1        75
#define PCL_COLUMN_2        PCL_COLUMN_1 + (PCL_PAPER_WIDTH/3)
#define PCL_COLUMN_3        (PCL_PAPER_WIDTH/2)
#define PCL_COLUMN_4        PCL_COLUMN_1 + (PCL_PAPER_WIDTH*2/3)
#define MM_TO_300DPI(x)     ((int)((x)*300/MM_PER_INCH))

#define KEY_MAIN_TITLE          "main_title"
#define KEY_MAIN_LOGO           "main_logo"
#define KEY_MENU_TITLE          "menu_title"
#define KEY_SUB_MENU_TITLE      "sub_menu_title"
#define KEY_ITEM                "item"
#define KEY_DATA                "data"
#define KEY_OFFSET_X            "offset_x"
#define KEY_POSITION_X          "x"
#define KEY_POSITION_Y          "y"
#define KEY_OUTLINE             "outline"
#define KEY_LOGO_IMAGE			"logo_image"
#define KEY_NEW_PAGE            "new_page"
#define KEY_ITEM_FONT_SIZE      "item_font_size"
#define KEY_BASE_X              "base_x"
#define KEY_PAGE_NUMBER         "page_number"
#define KEY_BARCODE             "barcode"
#define KEY_TONERCHART          "toner_chart"
#define KEY_DATETIME            "page_datetime"
#define KEY_FAXTHUMBNAIL        "fax_thumbnail"

#endif
