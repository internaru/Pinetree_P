/*
 * ============================================================================
 * (C) Copyright 2014   Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * @file map.c  
 *
 * @brief Implements an associative array using a hash table with nominal O(1) lookup.
 * 
 * The hash table is implementated using uthash, a third-party open-source C macro library
 * consisting of a single header file uthash.h located in .../third_party/uthash.
 * Documentation for uthash: http://troydhanson.github.io/uthash/userguide.html
 */

#include <stdint.h>
#include <stdbool.h>
#include "lassert.h"
#include "logger.h"
#include "debug.h" // for UNUSED_VAR
#include "memAPI.h"
#include "posix_ostools.h"
#include "pthread.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif // HAVE_CMD
#include "uthash.h" // hash table macros (third-party)
#include "map_api.h"

#define MAP_CONTEXT_SIGNATURE    0x484d4150 // ascii "HMAP"
#define MAP_ITERATOR_SIGNATURE   0x484c5354 // ascii "HLST"

/* re-define uthash mem allocation routines */
#undef uthash_malloc
#define uthash_malloc(sz) MEM_MALLOC(sz)
#undef uthash_free
#define uthash_free(ptr,sz) MEM_FREE_AND_NULL(ptr)
#undef uthash_fatal
#define uthash_fatal(msg) map_uthash_fatal(msg);

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "MAP: "
#define LOGGER_MODULE_MASK (DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT(23))  

#define DBG_ERR(...)        DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...)        DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
//#define DBG_ENTER(fmt,...)  dbg_printf(DBG_PRFX "==>%s "fmt"\n", __func__ ,##__VA_ARGS__ )
//#define DBG_RETURN(fmt,...) dbg_printf(DBG_PRFX "<==%s "fmt"\n", __func__ ,##__VA_ARGS__ )

#ifdef HAVE_CMD
#define DBG_CMD(...) cmd_printf( __VA_ARGS__ )
#endif // HAVE_CMD

#ifndef DBG_ERR 
    #define DBG_ERR(...)
#endif
#ifndef DBG_MSG
    #define DBG_MSG(...)
#endif
#ifndef DBG_ENTER
    #define DBG_ENTER(...)
#endif
#ifndef DBG_RETURN
    #define DBG_RETURN(...)
#endif


//=========================================================================================================================
// Data types
//=========================================================================================================================

typedef struct
{
    const void *user_data;
    UT_hash_handle hh;
} map_entry_t;

struct map_iterator_context_s;

typedef struct map_context_s 
{
#ifdef DEBUG
    uint32_t                signature;
#endif
    pthread_mutex_t         mtx;
    map_entry_t             *head; // for uthash
    struct map_context_s    *next; // for local map list
    struct map_iterator_context_s *iterators;
} map_context_t; 

typedef struct map_iterator_context_s
{
#ifdef DEBUG
    uint32_t                        signature;
#endif
    map_context_t                   *map_ctxt;
    map_entry_t                     *cur_entry;
    struct map_iterator_context_s   *next;
} map_iterator_context_t; 

static pthread_mutex_t g_map_list_mtx;
static map_context_t *g_map_list_head = NULL;
static uint32_t g_map_list_count = 0;

//=========================================================================================================================
// Local function prototypes
//=========================================================================================================================

static void map_uthash_fatal(char *uthash_msg);
static void map_list_insert_head(map_context_t *ctxt);
static void map_list_remove(map_context_t *ctxt);
static uint32_t map_list_get_count(void);
static map_context_t* map_list_get_by_enum(uint32_t map_enum);

#ifdef HAVE_CMD
static void map_register_dbg_cmds(void);
#endif // HAVE_CMD


//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================
#ifdef DEBUG 
# define MAP_LOCK_CONTEXT(ctxt)   DBG_XASSERT( 0 == pthread_mutex_lock(&ctxt->mtx), errno );	
# define MAP_UNLOCK_CONTEXT(ctxt) DBG_XASSERT( 0 == pthread_mutex_unlock(&ctxt->mtx), errno );	
# define MAP_LOCK_LIST()          DBG_XASSERT( 0 == pthread_mutex_lock(&g_map_list_mtx), errno );	
# define MAP_UNLOCK_LIST()        DBG_XASSERT( 0 == pthread_mutex_unlock(&g_map_list_mtx), errno );	
#else
# if 0 /// ignore error return in release code
#  define MAP_LOCK_CONTEXT(ctxt)   pthread_mutex_lock(&ctxt->mtx);
#  define MAP_UNLOCK_CONTEXT(ctxt) pthread_mutex_unlock(&ctxt->mtx);	
#  define MAP_LOCK_LIST()          pthread_mutex_lock(&g_map_list_mtx);	
#  define MAP_UNLOCK_LIST()        pthread_mutex_unlock(&g_map_list_mtx);	
# else /// release assert on non functional OS 
#  define MAP_LOCK_CONTEXT(ctxt)   REL_XASSERT( 0 == pthread_mutex_lock(&ctxt->mtx), errno );	
#  define MAP_UNLOCK_CONTEXT(ctxt) REL_XASSERT( 0 == pthread_mutex_unlock(&ctxt->mtx), errno );	
#  define MAP_LOCK_LIST()          REL_XASSERT( 0 == pthread_mutex_lock(&g_map_list_mtx), errno );	
#  define MAP_UNLOCK_LIST()        REL_XASSERT( 0 == pthread_mutex_unlock(&g_map_list_mtx), errno );	
# endif 
#endif


//=========================================================================================================================
// Module API
//=========================================================================================================================

int32_t map_init(void)
{
    DBG_ENTER();

    int32_t map_rcode = SYS_OK;

    static bool initialized = false;
    if(initialized)
    {
        goto done;
    }
    initialized = true; 

    int32_t posix_rcode;
    posix_rcode = posix_mutex_init(&g_map_list_mtx);
    if(posix_rcode)
    {
        DBG_ERR("error creating g_map_list_mtx (posix_rcode=%d)\n", posix_rcode);
        map_rcode = SYS_FAIL;
        goto done;
    }

    map_register_dbg_cmds();

done:
    DBG_RETURN("(map_rcode=%d)", map_rcode);
    return map_rcode;
}

map_handle_t map_create(uint32_t flags) 
{
    DBG_ENTER();

    XASSERT(flags == 0, flags); // flags currently not supported and should be 0

    int32_t posix_rcode;
    bool mtx_created = false;
    bool success = false;

    map_context_t *ctxt = NULL; 
    
    ctxt = (map_context_t *)MEM_MALLOC(sizeof(map_context_t));
    if(!ctxt)
    {
        goto done;
    }

    posix_rcode = posix_mutex_init(&ctxt->mtx);
    if(posix_rcode)
    {
        DBG_ERR("error creating map mutex (posix_rcode=%d)\n", posix_rcode);
        goto done;
    }
    mtx_created = true;

    // init map context
#ifdef DEBUG
    ctxt->signature = MAP_CONTEXT_SIGNATURE;
#endif
    ctxt->head = NULL;
    ctxt->next = NULL;
    ctxt->iterators = NULL;

    map_list_insert_head(ctxt);

    success = true;

done:
    if(!success)
    {
        // cleanup 
        if(ctxt)
        {
            if(mtx_created)
            {
                posix_rcode = pthread_mutex_destroy(&ctxt->mtx);
                ASSERT(posix_rcode == 0);
            }
            MEM_FREE_AND_NULL(ctxt);
        }
        DBG_ERR("create failed!\n");
    }
    DBG_RETURN();
    return (map_handle_t)ctxt;
}

int32_t map_insert(map_handle_t map, const void *key, uint32_t key_len, const void *value)
{
    DBG_ENTER();
    int32_t map_rcode = SYS_OK;
    map_context_t *ctxt = (map_context_t *)map;
#ifdef DEBUG
    XASSERT(ctxt->signature == MAP_CONTEXT_SIGNATURE, (int)ctxt);
#endif

    map_entry_t *entry, *tmp_entry;

    entry = (map_entry_t *)MEM_MALLOC(sizeof(map_entry_t));
    if(!entry)
    {
        DBG_ERR("add failed! low mem\n");
        map_rcode = MAP_LOW_MEM;
        goto done;
    }

    MAP_LOCK_CONTEXT(ctxt);

    // verify key uniqueness
    HASH_FIND(hh, ctxt->head, key, key_len, tmp_entry);
    if(tmp_entry)
    {
        // key already exists
        MAP_UNLOCK_CONTEXT(ctxt); 
        DBG_ERR("duplicate key detected! (key=%08x len=%d)\n", key, key_len);
        map_rcode = MAP_DUP_KEY;
        goto done;
    }

    entry->user_data = value;
    HASH_ADD_KEYPTR(hh, ctxt->head, key, key_len, entry);
    MAP_UNLOCK_CONTEXT(ctxt); 

done:
    if(map_rcode != SYS_OK)
    {
        MEM_FREE_AND_NULL(entry);
    }
    DBG_RETURN();
    return map_rcode;
}

void *map_lookup(map_handle_t map, const void *key, uint32_t key_len)
{
    DBG_ENTER();
    map_context_t *ctxt = (map_context_t *)map;
#ifdef DEBUG
    XASSERT(ctxt->signature == MAP_CONTEXT_SIGNATURE, (int)ctxt);
#endif

    MAP_LOCK_CONTEXT(ctxt);

    void *value = NULL;
    map_entry_t *entry;
    HASH_FIND(hh, ctxt->head, key, key_len, entry);
    if(entry)
    {
        value = (void *)entry->user_data;
    }

    MAP_UNLOCK_CONTEXT(ctxt);

    DBG_RETURN();
    return value;
}

void *map_remove(map_handle_t map, const void *key, uint32_t key_len)
{
    DBG_ENTER();
    map_context_t *ctxt = (map_context_t *)map;
#ifdef DEBUG
    XASSERT(ctxt->signature == MAP_CONTEXT_SIGNATURE, (int)ctxt);
#endif

    MAP_LOCK_CONTEXT(ctxt);

    void *value = NULL;
    map_entry_t *entry;
    HASH_FIND(hh, ctxt->head, key, key_len, entry);
    if(!entry)
    {
        // entry not found
        MAP_UNLOCK_CONTEXT(ctxt);
        goto done;
    }

    // if any iterators are pointing at this entry then advance them to next entry
    map_iterator_context_t *cur_iter = ctxt->iterators;
    while(cur_iter)
    {
        if(cur_iter->cur_entry == entry)
        {
            cur_iter->cur_entry = entry->hh.next;
            break;
        }
        cur_iter = cur_iter->next;
    }

    value = (void *)entry->user_data;
    HASH_DELETE(hh, ctxt->head, entry);

    MAP_UNLOCK_CONTEXT(ctxt);

    MEM_FREE_AND_NULL(entry);

done:
    DBG_RETURN();
    return value;
}

// TODO destroy is currently not thread safe, e.g. if one thread called destroy immediately after
// another thread called insert we would have a race condition. May need to convert handle from
// pointer to modulo index lookup if run into issues with map being used after destroyed.
void map_destroy(map_handle_t map)
{
    DBG_ENTER();
    map_context_t *ctxt = (map_context_t *)map;
#ifdef DEBUG
    XASSERT(ctxt->signature == MAP_CONTEXT_SIGNATURE, (int)ctxt);
#endif


#ifdef DEBUG
    MAP_LOCK_CONTEXT(ctxt);
    ctxt->signature = 0;
    MAP_UNLOCK_CONTEXT(ctxt);
#endif

    map_list_remove(ctxt);
    XASSERT(ctxt->next == NULL, (int)ctxt->next);

    // free all iterators
    map_iterator_context_t *cur_iter = ctxt->iterators;
    map_iterator_context_t *tmp_iter;
    ctxt->iterators = NULL;
    while(cur_iter)
    {
    #ifdef DEBUG
        XASSERT(cur_iter->signature == MAP_ITERATOR_SIGNATURE, (int)cur_iter);
        cur_iter->signature = 0;
    #endif
        tmp_iter = cur_iter;
        cur_iter = cur_iter->next;
        MEM_FREE_AND_NULL(tmp_iter);
    }

    // free all map entries
    map_entry_t *cur_entry, *tmp_entry;
    HASH_ITER(hh, ctxt->head, cur_entry, tmp_entry)
    {
        HASH_DELETE(hh, ctxt->head, cur_entry);
        MEM_FREE_AND_NULL(cur_entry);
    }

    // free the map
    int posix_rcode = pthread_mutex_destroy(&ctxt->mtx);
    DBG_ASSERT(posix_rcode == 0); UNUSED_VAR(posix_rcode);
    MEM_FREE_AND_NULL(ctxt);

    DBG_RETURN();
    return;
}

map_iterator_t map_iterate_begin(map_handle_t map)
{
    DBG_ENTER("(map=0x%08x)", (int)map);
    map_context_t *map_ctxt = (map_context_t *)map;
#ifdef DEBUG
    XASSERT(map_ctxt->signature == MAP_CONTEXT_SIGNATURE, (int)map_ctxt);
#endif

    map_iterator_context_t *iter_ctxt = NULL; 

    iter_ctxt = (map_iterator_context_t *)MEM_MALLOC(sizeof(map_iterator_context_t));
    if(!iter_ctxt)
    {
        XASSERT(iter_ctxt == MAP_INVALID_ITERATOR, (int)iter_ctxt);
        goto done;
    }

#ifdef DEBUG
    iter_ctxt->signature = MAP_ITERATOR_SIGNATURE;
#endif
    iter_ctxt->map_ctxt = map_ctxt;

    MAP_LOCK_CONTEXT(map_ctxt);
    iter_ctxt->cur_entry = map_ctxt->head;
    iter_ctxt->next = map_ctxt->iterators;
    map_ctxt->iterators = iter_ctxt;
    MAP_UNLOCK_CONTEXT(map_ctxt);

done:
    DBG_RETURN("(iter_ctxt=0x%08x)", (int)iter_ctxt);
    return (map_iterator_t)iter_ctxt;
}

void *map_iterate_first(map_iterator_t iterator)
{
    DBG_ENTER("(iter_ctxt=0x%08x)", (int)iterator);
    void *value = NULL;
    if(iterator == MAP_INVALID_ITERATOR)
    {
        goto done;
    }

    map_iterator_context_t *iter_ctxt = (map_iterator_context_t *)iterator; 
#ifdef DEBUG
    XASSERT(iter_ctxt->signature == MAP_ITERATOR_SIGNATURE, (int)iter_ctxt);
    iter_ctxt->signature = 0;
#endif

    ASSERT(iter_ctxt->map_ctxt);
    MAP_LOCK_CONTEXT(iter_ctxt->map_ctxt);
    iter_ctxt->cur_entry = iter_ctxt->map_ctxt->head;
    if(iter_ctxt->cur_entry)
    {
        value = (void *)iter_ctxt->cur_entry->user_data; 
    }
    MAP_UNLOCK_CONTEXT(iter_ctxt->map_ctxt);

done:
    DBG_RETURN();
    return value;
}

void *map_iterate_next(map_iterator_t iterator)
{
    DBG_ENTER("(iter_ctxt=0x%08x)", (int)iterator);
    void *value = NULL;
    if(iterator == MAP_INVALID_ITERATOR)
    {
        goto done;
    }

    map_iterator_context_t *iter_ctxt = (map_iterator_context_t *)iterator;
#ifdef DEBUG
    XASSERT(iter_ctxt->signature == MAP_ITERATOR_SIGNATURE, (int)iter_ctxt);
#endif

    ASSERT(iter_ctxt->map_ctxt);
    MAP_LOCK_CONTEXT(iter_ctxt->map_ctxt);
    if(iter_ctxt->cur_entry)
    {
        value = (void *)iter_ctxt->cur_entry->user_data; 
        iter_ctxt->cur_entry = iter_ctxt->cur_entry->hh.next;
    }
    MAP_UNLOCK_CONTEXT(iter_ctxt->map_ctxt);

done:
    DBG_RETURN();
    return value;
}

void map_iterate_end(map_iterator_t iterator)
{
    DBG_ENTER();
    if(iterator == MAP_INVALID_ITERATOR)
    {
        goto done;
    }

    map_iterator_context_t *iter_ctxt = (map_iterator_context_t *)iterator;
#ifdef DEBUG
    XASSERT(iter_ctxt->signature == MAP_ITERATOR_SIGNATURE, (int)iter_ctxt);
    iter_ctxt->signature = 0;
#endif

    // remove iterater from the list
    ASSERT(iter_ctxt->map_ctxt);
    MAP_LOCK_CONTEXT(iter_ctxt->map_ctxt);
    map_iterator_context_t *cur_ctxt = iter_ctxt->map_ctxt->iterators;
    map_iterator_context_t **prev = &iter_ctxt->map_ctxt->iterators;
    while(cur_ctxt)
    {
        if(cur_ctxt == iter_ctxt)
        {
            *prev = cur_ctxt->next;
            cur_ctxt->next = NULL;
            break;
        }
        prev = &cur_ctxt->next;
        cur_ctxt = cur_ctxt->next;
    }
    MAP_UNLOCK_CONTEXT(iter_ctxt->map_ctxt);

    // free the iterator
    MEM_FREE_AND_NULL(cur_ctxt);

done:
    DBG_RETURN();
    return;
}


//=========================================================================================================================
// Local helper functions
//=========================================================================================================================

static void map_uthash_fatal(char *uthash_msg)
{
    // TODO we should modify the uthash source code to continue working with the existing table size if
    // it is unable to allocate more memory.  Currently we get catastrophic failure and potential mem corruption.
    DBG_ERR("uthash fatal error!: %s", uthash_msg);
    ASSERT(0);
    return;
}

static void map_list_insert_head(map_context_t *ctxt)
{
    DBG_ENTER();
    ASSERT(ctxt);

    MAP_LOCK_LIST();

    ASSERT(!ctxt->next);
    ctxt->next = g_map_list_head;
    g_map_list_head = ctxt;
    g_map_list_count++;
    MAP_UNLOCK_LIST();

    DBG_RETURN();
    return;
}

static void map_list_remove(map_context_t *ctxt)
{
    DBG_ENTER();
    ASSERT(ctxt);

    MAP_LOCK_LIST();

    map_context_t *cur_ctxt = g_map_list_head;
    map_context_t **prev = &g_map_list_head;
    while(cur_ctxt)
    {
        if(cur_ctxt == ctxt)
        {
            // remove the item from the list
            *prev = cur_ctxt->next;
            cur_ctxt->next = NULL;
            break;
        }
        prev = &cur_ctxt->next;
        cur_ctxt = cur_ctxt->next;
    }
    ASSERT(g_map_list_count);
    g_map_list_count--;

    MAP_UNLOCK_LIST();

    DBG_RETURN();
    return;

}

static uint32_t map_list_get_count(void)
{
    return g_map_list_count;
}

static map_context_t *map_list_get_by_enum(uint32_t map_enum)
{
    int i = 0;
    map_context_t *ctxt = g_map_list_head;
    MAP_LOCK_LIST();
    if(map_enum >= g_map_list_count)
    {
        MAP_UNLOCK_LIST(); 
        return NULL;
    }
    while(ctxt && map_enum != i) 
    {
        ctxt = ctxt->next;
        i++;
    }
    MAP_UNLOCK_LIST(); 
    return ctxt;
}


//=========================================================================================================================
// Serial debug command functions
//=========================================================================================================================

#ifdef HAVE_CMD

static const char *map_cmd_test_usage =
    "<command>\n"\
    "       commands:\n"\
    "           create\n"\
    "           insert <map enum> <index of static key-value pair (0-3)>\n"\
    "           lookup <map enum> <key>\n"\
    "           remove <map enum> <key>\n"\
    "           destroy <map enum>\n";

static int map_list_cb( int argc, char *argv[] );
static int map_test_cb( int argc, char *argv[] );
static void cmd_hex_dump(unsigned char *ptr, int size);

static void map_register_dbg_cmds(void)
{
    DBG_ENTER();
    int retcode;
    retcode = cmd_register_cmd( "map",
                                NULL,
                                "Commands for testing and diagnosing associative arrays",
                                NULL,
                                NULL,
                                NULL );
    XASSERT( retcode==0, retcode );
    retcode = cmd_register_subcmd( "map",
                                   "list",
                                   "List currently instantiated associative arrays and their contents",
                                   "",
                                   NULL,
                                   map_list_cb );
    XASSERT( retcode==0, retcode );
    retcode = cmd_register_subcmd( "map",
                                   "test",
                                   "simple sanity testing of map implementation",
                                   map_cmd_test_usage,
                                   NULL,
                                   map_test_cb );
    XASSERT( retcode==0, retcode );
    UNUSED_VAR(retcode); 
    DBG_RETURN();
    return;
}

static int map_list_cb( int argc, char *argv[] )
{
    if(argc >2)
    {
        /* bad parameters */
        DBG_CMD("Usage: map list [enum]\n");
        return CMD_USAGE_ERROR;
    }
    else if(argc < 2)
    {
        /* return enumerated list of instantiated maps */
        int i = 0;
        map_context_t *ctxt = g_map_list_head;
        MAP_LOCK_LIST();
        uint32_t map_cnt = map_list_get_count();
        DBG_CMD("map instances (%d):\n", (int)map_cnt);
        while(ctxt)
        {
            DBG_CMD("%d: 0x%08x\n", i, (int)ctxt);
            ctxt = ctxt->next;
            i++;
        }
        MAP_UNLOCK_LIST();
        XASSERT(map_cnt == i, map_cnt);
    }
    else
    {
        /* return map info */
        uint32_t map_enum = atoi(argv[1]);
        MAP_LOCK_LIST();
        map_context_t *ctxt = map_list_get_by_enum(map_enum);
        if(!ctxt)
        {
            DBG_CMD("map enum %d not found!\n", (int)map_enum);
            MAP_UNLOCK_LIST(); 
            return CMD_OK;
        }
        unsigned int num_entries;
        num_entries = HASH_COUNT(ctxt->head);
        DBG_CMD("map enum %d has %d entries:\n", (int)map_enum, num_entries);
        map_entry_t *entry;
        int i = 0;
        for(entry = ctxt->head; entry != NULL; entry = entry->hh.next)
        {
            i++;
            DBG_CMD("(%d) key:0x%08x ==> val:0x%08x  key hex:\n", i, (int)entry->hh.key, (int)entry->user_data);
            cmd_hex_dump((unsigned char *)entry->hh.key, entry->hh.keylen);
        }
        MAP_UNLOCK_LIST(); 
    }

    return CMD_OK;
}

static int map_test_cb( int argc, char *argv[] )
{
    static struct {char *key; char *val;} test_vals[] = 
        {
            {"key0", "value0"},
            {"key1", "value1"},
            {"key2", "value2"},
            {"key3", "value3"},
        };

    if(argc > 5 || argc < 2)
    {
        /* bad parameters */
        return CMD_USAGE_ERROR;
    }
    else if(strncmp(argv[1], "create", 6) == 0)
    {
        if(argc != 2)
        {
            /* bad parameters */
            return CMD_USAGE_ERROR;
        }
        map_handle_t map; 
        map = map_create(0); 
        DBG_CMD("created map 0x%08x\n", (int)map);
        return CMD_OK; 
    }
    else if(strncmp(argv[1], "insert", 6) == 0)
    {
        if(argc != 4)
        {
            /* bad parameters */
            return CMD_USAGE_ERROR;
        }
        uint32_t map_enum = atoi(argv[2]);
        uint32_t test_idx = atoi(argv[3]);
        map_context_t *ctxt = map_list_get_by_enum(map_enum);
        if(!ctxt)
        {
            DBG_CMD("map enum %d not found!\n", (int)map_enum);
            return CMD_OK;
        }
        if(test_idx>3)
        {
            DBG_CMD("bad test index %d!\n", (int)test_idx);
            return CMD_OK;
        }
        int32_t map_rcode;
        map_rcode = map_insert((map_handle_t)ctxt, test_vals[test_idx].key, 
                               strlen(test_vals[test_idx].key), (void *)test_vals[test_idx].val);
        if(map_rcode != SYS_OK)
        {
            if(map_rcode == MAP_DUP_KEY) 
            {
                DBG_CMD("duplicate key '%s'!\n", test_vals[test_idx].key);
            }
            DBG_CMD("failed to add '%s' ==> '%s' to map %d!\n", 
                    test_vals[test_idx].key, test_vals[test_idx].val, (int)map_enum);
        }
        else
        {
            DBG_CMD("added to map %d:\nkey:0x%08x '%s' ==> val:0x%08x '%s'\n", 
                    (int)map_enum, (int)test_vals[test_idx].key, test_vals[test_idx].key,
                    (int)test_vals[test_idx].val, test_vals[test_idx].val);
        }
        return CMD_OK; 
    }
    else if(strncmp(argv[1], "lookup", 6) == 0)
    {
        if(argc != 4)
        {
            /* bad parameters */
            return CMD_USAGE_ERROR;
        }
        uint32_t map_enum = atoi(argv[2]);
        map_context_t *ctxt = map_list_get_by_enum(map_enum);
        if(!ctxt)
        {
            DBG_CMD("map enum %d not found!\n", (int)map_enum);
            return CMD_OK;
        }

        char *val;
        val = (char *)map_lookup((map_handle_t)ctxt, argv[3], strlen(argv[3]));
        if(!val)
        {
            DBG_CMD("key '%s' not found in map %d!\n", argv[3], (int)map_enum);
            return CMD_OK; 
        }
        DBG_CMD("key '%s' ==> val 0x%08x '%s'\n", argv[3], (int)val, val);

        return CMD_OK; 
    }
    else if(strncmp(argv[1], "remove", 6) == 0)
    {
        if(argc != 4)
        {
            /* bad parameters */
            return CMD_USAGE_ERROR;
        }
        uint32_t map_enum = atoi(argv[2]);
        map_context_t *ctxt = map_list_get_by_enum(map_enum);
        if(!ctxt)
        {
            DBG_CMD("map enum %d not found!\n", (int)map_enum);
            return CMD_OK;
        }

        char *val;
        val = (char *)map_remove((map_handle_t)ctxt, argv[3], strlen(argv[3]));
        if(!val)
        {
            DBG_CMD("key '%s' not found in map %d!\n", argv[3], (int)map_enum);
            return CMD_OK; 
        }
        DBG_CMD("removed from map %d:/nkey:'%s' ==> val:0x%08x '%s'\n", (int)map_enum, argv[3], (int)val, val);

        return CMD_OK; 
    }
    else if(strncmp(argv[1], "destroy", 7) == 0)
    {
        if(argc != 3)
        {
            /* bad parameters */
            return CMD_USAGE_ERROR;
        }
        uint32_t map_enum = atoi(argv[2]); 
        map_context_t *ctxt = map_list_get_by_enum(map_enum);
        if(!ctxt)
        {
            DBG_CMD("map enum %d not found!\n", (int)map_enum);
            return CMD_OK;
        }
        map_destroy((map_handle_t)ctxt); 
        DBG_CMD("destroyed map %d: 0x%08x\n", (int)map_enum, (int)ctxt);
        return CMD_OK; 
    }
    else
    {
        /* bad parameters */
        return CMD_USAGE_ERROR;
    }
    return CMD_OK; 
}

#include <ctype.h> // for isprint()
static void cmd_hex_dump( unsigned char *ptr, int size ) 
{
    static char hex_ascii[] = 
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    int i;
    unsigned char line[80];
    unsigned char *ascii, *hex;
    unsigned char *endptr;

    endptr = ptr + size;
    memset( line, ' ', 80 );
    line[69] = 0;
    while( ptr != endptr ) {
        hex = &line[2];
        ascii = &line[52];
        for( i = 0; i < 16; i++ ) {
            if( isprint( *ptr ) ) {
                *ascii++ = *ptr;
            }
            else {
                *ascii++ = '.';
            }
            *hex++ = hex_ascii[*ptr >> 4];
            *hex++ = hex_ascii[*ptr & 0x0f];
            *hex++ = ' ';
            ptr++;
            if( ptr == endptr ) {
                /* clean out whatever is left from the last line */ 
                memset( hex, ' ', ( 15 - i ) * 3 );
                memset( ascii, ' ', 15 - i );
                /* i+1 so we can find the starting pointer correctly in our
                 * ptr-i in the printf below
                 */
                i++;
                break;
            }
        }
        DBG_CMD("%s\n", line );
    }
}

#endif // HAVE_CMD

