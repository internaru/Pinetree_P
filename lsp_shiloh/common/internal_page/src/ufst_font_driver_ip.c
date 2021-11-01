
/* 
 *
 * ============================================================================
 * Copyright (c) 2009   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#include "memAPI.h"

#include "cgconfig.h"     /* this must be first  */
#include "ufstport.h"     /* this must be second */
#include "shareinc.h"     /* then this ufst include probably should be third */

#include "lassert.h"
#include "rom_id_mapping.h"
#include "string.h"
#include "checksum.h"

#include "agprint.h"
#include "dprintf.h"
#include "logger.h"
#include "debug.h"
#include "rommgrapi.h"
#include "ufst_font_driver_ip.h"
#include "string_mgr_api.h"

static bool initialized = false;
static string_mgr_language_t CurLang=-1;
static uint32_t CurSize=0;
static bool CurRotate=false;
static IF_STATE if_state_cp;
static IFCONFIG config_block;
static FONTCONTEXT fontcontext;
static uint32_t *page_ptrs[3]={0,0,0};
int ipfatStrokes = 0x00001000;

static LPUB8 load_font_from_lang( string_mgr_language_t lang )
{
    UINT32 fontSize = 0; 
    uint8_t *ptr = 0;

    /// NB revisit todo:
    // add table of lang to font location! customize per product?
    rommgr_id_mapping_t loc = ROMMGR_ENGLISH_FONT; // revisit currently chinese_Simplified and latin. 

    if ( lang != LANG_ENGLISH )  // revisit perhaps this should be a larger language set.
        loc = ROMMGR_SIMP_CHINESE;    // currently arabic.

    // if (lang == LANG_SIMPCHINESE)

    // default to extended latin font

    // ptr = RomMgr(loc, &fontSize);
    ptr = (uint8_t*) RomMgrReadData( IM_DATA_SECTOR, loc, &fontSize);
    DPRINTF((DBG_OUTPUT|DBG_LOUD),
            ("Font: Load file, addr[0]=%x  fontSize=%d checksum=%d\n", 
             ptr, 
             fontSize,
             CalcChecksum( (unsigned short*)ptr, fontSize))); 

    ASSERT( ptr && fontSize != 0 );

    return (LPUB8)ptr;
}
void ufst_font_driver_internal_pages_halt(void)
{
    uint32_t i;
    CGIFexit (&if_state_cp);
    MEM_FREE_AND_NULL(fontcontext.font_hdr);
    for(i = 0; i < 3; i++)
    {
        if(page_ptrs[i])
            MEM_FREE_AND_NULL(page_ptrs[i]);
    }
    CurLang = -1;
    CurSize = 0;    // reset the current font settings.
    initialized = false;
}

/// One time initialization called once per client/thread.
/// each thread or client must have it's own if_state 
void
ufst_font_driver_internal_pages_init(string_mgr_language_t lang, uint32_t resolution_x, uint32_t resolution_y)
{
    uint32_t res_x, res_y;
    char *ptr;
    UW16 handle;

    if (initialized == false) 
    {
        initialized = true;
        CurSize = 0;    // reset the current font settings.

        if ( CGIFinit( &if_state_cp ) )
            ASSERT(0);

        memset(&config_block, 0, sizeof(config_block));
        config_block.bit_map_width = RASTER_ORG;
#if CACHE
        config_block.max_char_size = 5000;
#endif
    
        if ( CGIFconfig( &if_state_cp, &config_block) )
            ASSERT(0);

        ptr =  (char*) MEM_MALLOC( 20000 );
        page_ptrs[0] = (uint32_t *)ptr; // save the ptr for release later.
        if (CGIFfund( &if_state_cp, (UW16)CACHE_POOL, (LPSB8)ptr, (UL32)20000l, (LPUW16)&handle )) {
            ASSERT(0);
            return;
        }
        ptr = (char*) MEM_MALLOC( 20000 );  // save the ptr for release later.
        page_ptrs[1] = (uint32_t *) ptr;
        if (CGIFfund( &if_state_cp, (UW16)BUFFER_POOL, (LPSB8)ptr, 20000l, &handle )) {
            ASSERT(0);
            return;
        }
        ptr = (char*) MEM_MALLOC( 20000 );
        page_ptrs[2] = (uint32_t *) ptr;    // save the ptr for release later.
        if (CGIFfund( &if_state_cp, CHARGEN_POOL, (LPSB8)ptr, 20000l, &handle )) {
            ASSERT(0);
            return;
        }
#if 0
        if (0) // do we need this symbol set?
        {
            LPUB8  ss_addr[3]; 
            UB8   *iffnt_addr = 0;    
            UINT32 fontSize = 0; 
            ss_addr[0] = NULL;
            ss_addr[1] = NULL;
            ss_addr[2] = NULL;
            UW16 status;
        

            //load tt symbol set     
            ss_addr[0] = (uint8_t*) RomMgrReadData( IM_DATA_SECTOR,(uint32_t) ROMMGR_ENGLISH_FONT, &fontSize);
            
            ASSERT(ss_addr[0]);
            DPRINTF((DBG_OUTPUT|DBG_LOUD),
                    ("Font: Load tt symbol set fle, ss_addr[0]=%x  fontSize=%d\n", ss_addr, fontSize)); 
            
            status = CGIFinitRomInfo(&if_state_cp, iffnt_addr, &ss_addr[0] );
            ASSERT(!status);  
        }
#endif   
        if ( CGIFenter( &if_state_cp ) )
            ASSERT(0);
        CGIFfont_access(&if_state_cp, ROM_ACCESS);

    
        fontcontext.font_id = 0;
        fontcontext.xspot = F_ONE;
        fontcontext.yspot = F_ONE;

    
        res_x = 23622; 
        res_y = 23622; 
        // res = 1000mm / 25.4 mm/" * 600 dpi -> 23622 
        res_x = (1000 * resolution_x)/25.4;
        res_y = (1000 * resolution_y)/25.4;

        fontcontext.xres = res_x;
        fontcontext.yres = res_y;
        fontcontext.shear.x      = 0;           /* no shear */
        fontcontext.shear.y      = F_ONE;       /*   "      */
        fontcontext.rotate.x     = F_ONE;       /* no rotation */
        fontcontext.rotate.y     = 0;           /*      "      */
        
        fontcontext.ssnum        = UNICODE;   /* desktop */
        fontcontext.format       = FC_TT_TYPE 
            | FC_ROM_TYPE   /* bitmap, TrueType */
            | FC_NON_Z_WIND ;   /* NON_ZERO Winding */
        
        fontcontext.ExtndFlags   = 0;
        
#if NO_SYMSET_MAPPING
        fontcontext.dl_ssnum &= 0xff00;
        fontcontext.dl_ssnum |= (3 | (1 << 4));
        fontcontext.ExtndFlags   |= EF_NOSYMSETMAP;
#endif  /* NO_SYMSET_MAPPING */


        fontcontext.stroke_pct = ipfatStrokes ; // 0x00001000; // 0;?


 //       ufst_font_driver_ip_setup_font( lang, 16, false );
    }
}


IF_STATE *ufst_font_driver_internal_pages_set_font()
{
    CGIFfont( &if_state_cp, &fontcontext );
    return &if_state_cp;
} 

void ufst_font_driver_ip_setup_font( IF_STATE **ifs, string_mgr_language_t lang, uint32_t size_pts, bool Rotate )
{
    if((CurLang == lang) &&
       (size_pts == CurSize) &&
       (CurRotate == Rotate))
        return;
    CurSize = size_pts;
    CurRotate = Rotate;
    fontcontext.point_size = size_pts * 8;  // eighth of a point units
    fontcontext.set_size = size_pts * 8;
    if(Rotate)
    {
        fontcontext.rotate.x = 0;       // rotate the font
        fontcontext.rotate.y = -1 * F_ONE;
    } else
    {
        fontcontext.rotate.x = F_ONE;   // no rotation
        fontcontext.rotate.y = 0;
    }

    if (lang != CurLang) {
        if ( fontcontext.font_hdr ) {
            MEM_FREE_AND_NULL(fontcontext.font_hdr);
        }
        fontcontext.font_hdr = load_font_from_lang( lang );
    }
    CurLang = lang;
    *ifs = ufst_font_driver_internal_pages_set_font();
    return;
}

int32_t char_escape( PIFBITMAP pbm)
{
    /* use C language floating point for convenience. */
    double pt_per;   /* points per inch or meter */
    double esc;
    PFONTCONTEXT fc = &fontcontext;

    if (fc->format & FC_INCHES_TYPE)
        pt_per = 1.0;           /* resolution dots per inch */
    else
        pt_per = 39.37;         /* resolution dots per meter */
#if DU_ESCAPEMENT
    if (pbm->du_emx == 8782)
        pt_per /= .01383;   /* 72.307 points per inch */
    else
        pt_per *= 72.0;     /* 72 points per inch */
#else
    pt_per /= .01383;       /* 72.307 points per inch */
#endif
    if (fc->format & FC_16THS_TYPE)
        esc = fc->set_size / 16.0;
    else
        esc = fc->set_size / 8.0;

    esc *= ((double)pbm->escapement * fc->xres / (pt_per * pbm->du_emx));

    return (uint32_t)esc; // truncate  or // (floor((esc * 65536.0 + 0.5)));  /* convert to 16.16 fixed point */
}

void getCharBmp(IF_STATE *ifs, uint32_t chId, PPIFBITMAP ppbm) 
{
    int status; 

    status = CGIFchar(ifs, chId, ppbm, (SW16)0);

    if (status)
    {
        DPRINTF((DBG_OUTPUT|DBG_LOUD), ("CGIFchar error %d, chId=%04x\n", status, chId));
        // ASSERT(0); 
        chId = '?';
        status = CGIFchar(ifs, chId, ppbm, (SW16)0);
        if (status) {
            chId = ' ';
            status = CGIFchar(ifs, chId, ppbm, (SW16)0);
            DPRINTF((DBG_OUTPUT|DBG_LOUD), ("CGIFchar error %d, chId=%04x\n", status, chId));
        }
    }
}


