#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "ctemplate.h"
#include "report_page.h"
#include "json_to_pcl.h"
#include "json_common.h"
#include "test_templates.h"

void test(void)
{
    ReportPageGenerator report;

    char fw_version[64];
    char serial_number[64];
    char duplex[64];
    char offset_x[64];
    char offset_y[64];
    char tray_no[64];

    strcpy(fw_version, "abc");
    strcpy(serial_number, "abc");
    strcpy(duplex, "abc");
    strcpy(offset_x, "abc");
    strcpy(offset_y, "abc");
    strcpy(tray_no, "abc");

	ReportPageOpenGenerator(&report, "test.json");

    ReportPageSetItemFontSize(&report, 13);
    
    ReportPageDrawOutline(&report);
    ReportPageAddMainTitle(&report, "KARA/KARA-S", 100, 100);

    ReportPageAddMenu(&report, "System", 100, 300);
    //ReportPageAddSubMenu(&report, "sub menu");

    ReportPageStartItem(&report);
    ReportPageAddItem(&report, 20, "fw version");
    ReportPageAddItem(&report, 500, fw_version);
    ReportPageEndItem(&report);

    ReportPageStartItem(&report);
    ReportPageAddItem(&report, 20, "serial number");
    ReportPageAddItem(&report, 500, serial_number);
    ReportPageEndItem(&report);

    ReportPageStartItem(&report);
    ReportPageAddItem(&report, 20, "duplex");
    ReportPageAddItem(&report, 500, duplex);
    ReportPageEndItem(&report);

    ReportPageStartItem(&report);
    ReportPageAddItem(&report, 20, "offset x");
    ReportPageAddItem(&report, 500, offset_x);
    ReportPageEndItem(&report);

    ReportPageStartItem(&report);
    ReportPageAddItem(&report, 20, "offset y");
    ReportPageAddItem(&report, 500, offset_y);
    ReportPageEndItem(&report);

    ReportPageStartItem(&report);
    ReportPageAddItem(&report, 20, "tray no");
    ReportPageAddItem(&report, 500, tray_no);
    ReportPageEndItem(&report);

	ReportPageCloseGenerator(&report);

	PclConvertJsonToPcl("test.json", "test.prn");
}


void main(void) {
    TMPL_varlist *varlist;
    TMPL_fmtlist *fmtlist;

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);
    varlist = TMPL_add_var(0, "c1", "20", 0);
    varlist = TMPL_add_var(varlist, "c2", "500", 0);
    varlist = TMPL_add_var(varlist, "ip", "1.1", 0);
    varlist = TMPL_add_var(varlist, "netmask", "2.2", 0);
    varlist = TMPL_add_var(varlist, "gateway", "3.3", 0);
    varlist = TMPL_add_var(varlist, "mac_address", "44", 0);
    
    FILE *fout = fopen("test.json", "w");
    TMPL_write(0, test_tmpl, fmtlist, varlist, fout, stderr);
    TMPL_free_varlist(varlist);
    TMPL_free_fmtlist(fmtlist);
    fclose(fout);

    PclConvertJsonToPcl("test.json", "test.pcl");
}
