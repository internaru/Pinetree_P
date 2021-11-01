#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "report_page.h"
#include "json_common.h"

int yajl_gen_key_to_string_map(yajl_gen g, const char *key, const char *value)
{
    yajl_gen_string(g, (const unsigned char *)key, strlen(key));
    yajl_gen_string(g, (const unsigned char *)value, strlen(value));
    return 0;
}

int yajl_gen_key_to_integer_map(yajl_gen g, const char *key, int value)
{
    yajl_gen_string(g, (const unsigned char *)key, strlen(key));
    yajl_gen_integer(g, value);
    return 0;
}

int yajl_gen_key_to_bool_map(yajl_gen g, const char *key, int value)
{
    yajl_gen_string(g, (const unsigned char *)key, strlen(key));
    yajl_gen_bool(g, value);
    return 0;
}

int ReportPageOpenGenerator(ReportPageGenerator *generator, const char *json_name)
{
    yajl_gen_config conf = { 1, "  " };
    generator->json = fopen(json_name, "w");
    generator->g = yajl_gen_alloc(&conf, NULL);
    yajl_gen_map_open(generator->g);
    return 0;
}

int ReportPageCloseGenerator(ReportPageGenerator *generator)
{
    const unsigned char * buf = 0;
    unsigned int len = 0;

    yajl_gen_map_close(generator->g);
    yajl_gen_get_buf(generator->g, &buf, &len);
    fwrite(buf, 1, len, generator->json);
    yajl_gen_clear(generator->g);
    fseek(generator->json, 0, SEEK_SET);

    fclose(generator->json);
    yajl_gen_free(generator->g);
    return 0;
}

int ReportPageAddMainTitle(ReportPageGenerator *generator, const char *title, int x, int y)
{
    ReportPageSetPosition(generator, x, y);
    yajl_gen_key_to_string_map(generator->g, KEY_MAIN_TITLE, title);
    return 0;
}

int ReportPageAddMenu(ReportPageGenerator *generator, const char *title, int x, int y)
{
    ReportPageSetPosition(generator, x, y);
    yajl_gen_key_to_string_map(generator->g, KEY_MENU_TITLE, title);
    return 0;
}

int ReportPageAddSubMenu(ReportPageGenerator *generator, const char *title)
{
    yajl_gen_key_to_string_map(generator->g, KEY_SUB_MENU_TITLE, title);
    return 0;
}

int ReportPageSetPosition(ReportPageGenerator *generator, int x, int y)
{
    yajl_gen_key_to_integer_map(generator->g, KEY_POSITION_X, x);
    yajl_gen_key_to_integer_map(generator->g, KEY_POSITION_Y, y);
    return 0;
}

int ReportPageStartItem(ReportPageGenerator *generator)
{
    yajl_gen_string(generator->g, (const unsigned char *)KEY_ITEM, strlen(KEY_ITEM));
    yajl_gen_array_open(generator->g);
    return 0;
}

int ReportPageEndItem(ReportPageGenerator *generator)
{
    yajl_gen_array_close(generator->g);
    return 0;
}

int ReportPageAddItem(ReportPageGenerator *generator, int x_offset, const char *data)
{
    yajl_gen_map_open(generator->g);
    yajl_gen_string(generator->g, (const unsigned char *)KEY_OFFSET_X, strlen(KEY_OFFSET_X));
    yajl_gen_integer(generator->g, x_offset);
    yajl_gen_string(generator->g, (const unsigned char *)KEY_DATA, strlen(KEY_DATA));
    yajl_gen_string(generator->g, (const unsigned char *)data, strlen(data));
    yajl_gen_map_close(generator->g);
    return 0;
}

int ReportPageAddNewPage(ReportPageGenerator *generator)
{
    yajl_gen_key_to_bool_map(generator->g, KEY_NEW_PAGE, 1);
    return 0;
}

int ReportPageDrawOutline(ReportPageGenerator *generator)
{
    yajl_gen_key_to_bool_map(generator->g, KEY_OUTLINE, 1);
    return 0;
}

int ReportPageAddLogoImage(ReportPageGenerator *generator)
{
	yajl_gen_key_to_bool_map(generator->g, KEY_LOGO_IMAGE, 1);
	return 0;
}

int ReportPageSetItemFontSize(ReportPageGenerator *generator, int size)
{
    yajl_gen_key_to_integer_map(generator->g, KEY_ITEM_FONT_SIZE, size);
    return 0;
}
