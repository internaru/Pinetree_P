#ifndef MENU_PAGE_H
#define MENU_PAGE_H

#include "api/yajl_gen.h"

typedef struct
{
    FILE *json;
    yajl_gen g;
} ReportPageGenerator;

int ReportPageOpenGenerator(ReportPageGenerator *generator, const char *json_name);
int ReportPageCloseGenerator(ReportPageGenerator *generator);
int ReportPageAddMainTitle(ReportPageGenerator *generator, const char *title, int x, int y);
int ReportPageAddMenu(ReportPageGenerator *generator, const char *title, int x, int y);
int ReportPageAddSubMenu(ReportPageGenerator *generator, const char *title);
int ReportPageSetPosition(ReportPageGenerator *generator, int x, int y);
int ReportPageStartItem(ReportPageGenerator *generator);
int ReportPageEndItem(ReportPageGenerator *generator);
int ReportPageAddItem(ReportPageGenerator *generator, int x_offset, const char *data);
int ReportPageAddNewPage(ReportPageGenerator *generator);
int ReportPageDrawOutline(ReportPageGenerator *generator);
int ReportPageAddLogoImage(ReportPageGenerator *generator);
int ReportPageSetItemFontSize(ReportPageGenerator *generator, int size);

#endif
