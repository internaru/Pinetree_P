/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/* \file cmd_proc.c
 *
 * \brief Clone(ish) of firmware command line cmd_proc functions.
 *
 * Needed for linux-kernel debug capabilities. Built using Chimera for easy
 * testing/debugging. Just enough fo the cmd functions to allow scan firmware
 * to build/run.
 *
 * davep 12-Sep-2012
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "scos.h"

#include "lassert.h"
#include "cmd_proc_api.h"
#include "utils.h"
#include "memAPI.h"
#include "dprintf.h"

//#define CMD_DEBUG 2

#ifdef CMD_DEBUG
    #define dbg1 cmd_printf
    #if CMD_DEBUG > 1
        #define dbg2 cmd_printf
    #else
        #define dbg2(...)
    #endif
#else
    #define dbg1(...)
    #define dbg2(...)
#endif

#define MAX_CMD_NAME 32
#define MAX_NUM_CMD 8
#define MAX_CMD_LINE 138

#ifdef __KERNEL__
    #define v_printit(fmt,ap) vprintk(fmt,ap)
#else
    #define v_printit(fmt,ap) minVsprintf(NULL,fmt,ap)
#endif

struct cmd {
    char name[MAX_CMD_NAME+1];
    int namelen;

    cmd_c_proc f_proc;

    /* peer of current node (linked list) */
    struct cmd *next;

    struct cmd *child;
};

static struct cmd base_list[MAX_NUM_CMD]; 

static void print_cmd_node( const char *prefix, struct cmd * const node )
{
    ASSERT( node!= NULL );

    dbg1( "%s node=%p name=\"%s\" len=%d next=%p child=%p proc=%p\n", 
            prefix, node,
            node->name, node->namelen,
            node->next, node->child, node->f_proc );
}

static struct cmd *find_cmd_by_name( struct cmd *cmd_list, const char *name ) 
{
    int i;
    int idx;

    dbg2( "%s name=\"%s\"\n", __FUNCTION__, name );

    idx = -1;

    /* O(n) is a bummer */
    for( i=0 ; i<MAX_NUM_CMD ; i++ ) {
        if( cmd_list[i].namelen==0 ) {
            /* empty slot */
            continue;
        }

        if( str_match( cmd_list[i].name, name, cmd_list[i].namelen ) ) {
            idx = i;
            break;
        }
    }
    if( idx==-1 ) {
        return NULL;
    }

    return &cmd_list[idx];
}

static struct cmd *find_empty( struct cmd *cmd_list ) 
{
    int i;
    int idx;

    idx = -1;
    for( i=0 ; i<MAX_NUM_CMD ; i++ ) {
        if( base_list[i].namelen==0 ) {
            /* empty element ; use it */
            idx = i;
            break;
        }
    }
    if( idx==-1 ) {
        return NULL;
    }

    return &cmd_list[idx];
}

static struct cmd* find_node_in_list_using_name( struct cmd *root, const char *name )
{
    struct cmd *curr;

    dbg2( "%s root=%p rootname=\"%s\" name=\"%s\"\n", __FUNCTION__, root,
            root?root->name:"(null)", name );

    /* linear search the linked list looking for the matching name */
    curr = root;
    while( curr != NULL ) {
        if( str_match( curr->name, name, curr->namelen ) ) {
            dbg2( "%s found \"%s\"\n", __FUNCTION__, name );
            break;
        }
        curr = curr->next;
    }
    if( !curr ) {
        dbg2( "%s name=\"%s\" not found\n", __FUNCTION__, name );
    }

    return curr;
}

static struct cmd *find_node_using_namelist( struct cmd *root, 
                                            char *namelist[],
                                            int nameidx )
{
    struct cmd *curr;

    dbg2( "%s root=%p find=\"%s\"\n", __FUNCTION__, root, namelist[nameidx] );

    if( root==NULL ) {
        return NULL;
    }

    /* if we are at the end of our command list, nowhere left to search */
    if( namelist[nameidx]==NULL ) {
        return root;
    }

    /* find the child node with the matching name */
    curr = find_node_in_list_using_name( root->child, namelist[nameidx] );
    if( curr==NULL ) {
        /* root is our best match */
        return root;
    }

    /* stomp the string indicating we found it; cheap hack to track the depth
     * so we can mimic the bizarre-o behavior of the current cmd_proc.c which
     * only passes argc/argv of the remaining fields after we find a match.
     */
    namelist[nameidx] = NULL;

    /* recursion -- eek! */
    return find_node_using_namelist( curr, namelist, nameidx+1 );
}

static struct cmd *new_cmd_node( const char *name ) 
{
    struct cmd *node;

    node = MEM_MALLOC( sizeof(struct cmd) );
    if( !node ) {
        return NULL;
    }
    memset( node, 0, sizeof(struct cmd) );
    node->namelen = strnlen( name, MAX_CMD_NAME );

    /* name should have been sanity checked before we get here */
    XASSERT( node->namelen>0 && node->namelen<MAX_CMD_NAME, node->namelen );

    strncpy( node->name, name, MAX_CMD_NAME );

    return node;
}

static struct cmd * add_new_cmd_node( struct cmd *root, const char *name )
{
    struct cmd *node;

    node = new_cmd_node( name );
    if( node==NULL ) {
        return NULL;
    }
    
    /* add to the head of root's linked list */
    node->next = root->child;
    root->child = node;

    return node;
}

int cmd_register_cmd(const char* name, const char* shortalias,
                     const char* desc, const char* usage, const char* notes,
                     cmd_c_proc  proc)
{
    int namelen;
    struct cmd *node;
    
    dbg2( "%s name=%s proc=%p\n", __FUNCTION__, name, proc );

    namelen = strnlen( name, MAX_CMD_NAME );
    if( namelen==0 || namelen >= MAX_CMD_NAME ) {
        /* invalid name */
        return CMD_ERROR;
    }

    /* already registered? */
    node = find_cmd_by_name( base_list, name );
    if( node!=NULL ) {
        /* already registered */
        return CMD_ERROR;
    }

    node = find_empty( base_list );
    if( !node ) {
        /* no room at the inn */
        return CMD_ERROR;
    }

    /* found space! use it */
    memset( node, 0, sizeof(struct cmd) );
    strncpy( node->name, name, MAX_CMD_NAME );
    node->namelen = namelen;
    node->f_proc = proc;
    
    return CMD_OK;
}

int cmd_register_subcmd(const char* cmdname, const char* subname,
                const char* desc, const char* usage, const char* notes,
                cmd_c_proc  proc)
{
    int i;
    int cmdnamelen, subnamelen;
    int num_cmds;
    char basecmd[MAX_CMD_LINE+1];
    char *cmdlist[20];
    struct cmd *root, *node, *new_node;

    dbg2( "%s cmdname=\"%s\" subname=\"%s\" proc=%p\n", 
            __FUNCTION__, cmdname, subname, proc );

    /* sanity check the incoming names */
    subnamelen = strnlen( subname, MAX_CMD_NAME );
    if( subnamelen==0 || subnamelen >= MAX_CMD_NAME ) {
        /* invalid subname */
        return CMD_ERROR;
    }

    /* The cmdname can be a space separated string, essentially a command line
     * itself. Have to split it then search the tree for each substring. 
     */
    cmdnamelen = strnlen( cmdname, MAX_CMD_LINE );
    if( cmdnamelen==0 || cmdnamelen >= MAX_CMD_LINE ) {
        return CMD_ERROR;
    }

    /* split the possible space containing cmdname into an array of pointers.
     * Validate said array of pointers. 
     *
     * str_split() is destructive so use a copy (on the stack) 
     */
    memset( basecmd, 0, sizeof(basecmd) );
    strncpy( basecmd, cmdname, MAX_CMD_LINE );
    num_cmds = str_split( basecmd, cmdnamelen, cmdlist, 20, " " );
    if( num_cmds < 1 ) {
        return CMD_ERROR;
    }

    /* validate all the strings in the cmdname */
    for( i=0 ; i<num_cmds ; i++ ) {
        cmdnamelen = strnlen( cmdlist[i], MAX_CMD_NAME );
        if( cmdnamelen==0 || cmdnamelen >= MAX_CMD_NAME ) {
            /* invalid cmdname */
            return CMD_ERROR;
        }
    }

    /* find the base command in our global array */
    root = find_cmd_by_name( base_list, cmdlist[0] );
    if( root == NULL ) {
        /* not found */
        return CMD_ERROR;
    }

    dbg2( "%s found name=\"%s\" node=%p\n", __FUNCTION__, cmdlist[0], root );

    /* Search the cmd's tree for the best match to the cmd list we have. By
     * design, we should at least always find the root.
     */
    node = find_node_using_namelist( root, cmdlist, 1 );
    ASSERT( node );
    print_cmd_node( "found", node );

    /* check the subname is not already in the list */
    if( find_node_in_list_using_name( node->child, subname ) != NULL ) {
        return CMD_ERROR;
    }

    /* the subname is new so add */
    new_node = add_new_cmd_node( node, subname );
    if( !new_node ) {
        /* probably out of memory */
        return CMD_ERROR;
    }
    /* fill in the fields we want */
    new_node->f_proc = proc;

    print_cmd_node( "new_node", new_node );
    print_cmd_node( "parent node", node );

    return CMD_OK;
}

int cmd_printf(const char* format,...)
{
    va_list ap;
    int len;

    va_start( ap, format ); 
    len = v_printit( (char *)format, ap );
    va_end( ap );
    return len;
}

int cmd_append_result(const char* format,...) 
{
    va_list ap;
    int len;

    va_start( ap, format ); 
    len = v_printit( (char *)format, ap );
    va_end( ap ); 
    return len;
}

int cmd_line_run( const char *cmdline ) 
{
    int retcode, cmdline_len, num_cmds;
    char cmdline_copy[MAX_CMD_LINE+1];
    struct cmd *node, *root;
    char *cmdlist[20];
    int i, argc;
    char **argv;

//    cmd_printf( "%s \"%s\"\n", __FUNCTION__, cmdline );

    cmdline_len = strnlen( cmdline, MAX_CMD_LINE );
    if( cmdline_len==0 || cmdline_len >= MAX_CMD_LINE ) {
        dbg2( "%s bad length len=%d (min=0 max=%d)\n", __FUNCTION__, 
                cmdline_len, MAX_CMD_LINE );
        return CMD_ERROR;
    }

    /* str_split() is destructive so use a copy (on the stack) */
    memset( cmdline_copy, 0, sizeof(cmdline_copy) );
    strncpy( cmdline_copy, cmdline, MAX_CMD_LINE );
    cmdline_copy[MAX_CMD_LINE] = 0;

    /* davep 18-Oct-2012 ; kill tabs, end-of-line, other evil characters */
    str_tr( cmdline_copy, "\t\r\n", "   ", cmdline_len );

    /* davep 18-Oct-2012 ; kill leading/trailing whitespace */
    trim( cmdline_copy );

    /* re-calculate the length after trim()'ing */
    cmdline_len = strnlen( cmdline_copy, MAX_CMD_LINE );
    if( cmdline_len==0 || cmdline_len >= MAX_CMD_LINE ) {
        dbg2( "%s bad length len=%d (min=0 max=%d)\n", __FUNCTION__, 
                cmdline_len, MAX_CMD_LINE );
        return CMD_ERROR;
    }

//    hex_dump( cmdline_copy, MAX_CMD_LINE );

    num_cmds = str_split( cmdline_copy, cmdline_len, cmdlist, 20, " \t" );
    if( num_cmds < 1 ) {
        return CMD_ERROR;
    }

    root = find_cmd_by_name( base_list, cmdlist[0] );
    if( root == NULL ) {
        /* not found */
        dbg2( "%s cmd \"%s\" not found\n", __FUNCTION__, cmdlist[0] );
        return CMD_ERROR;
    }

    node = find_node_using_namelist( root, cmdlist, 1 );
    if( node==NULL ) {
        /* not found */
        return CMD_ERROR;
    }

    retcode = CMD_OK;

    print_cmd_node( "found", node );

    if( !node->f_proc ) {
        return CMD_ERROR;
    }

    /* count the remaining fields after what we were able to match
     * (find_node_using_namelist() will stomp on cmdlist as it finds matches)
     *
     * turns out the callback functions don't use the same semantics as
     * main()'s argc/argv. argc doesn't include the command name itself BUT I
     * think argv[] does. That's just great.
     */
    argc = num_cmds;
    argv = cmdlist;
    for( i=0 ; i<num_cmds ; i++ ) {
        if( cmdlist[i]==NULL ) {
            argc -= 1;
            argv += 1;
        }
    }

    /* XXX argv[0] is weird; my current scan firmware doesn't use argv[0] but
     * there might be some code that expects it to be something useful. Not
     * sure what to put in here until I can explore how the ThreadX version of
     * cmd_proc behaves.
     */
//    argv[0] = cmdline;

    dbg2( "%s argc=%d\n", __FUNCTION__, argc );

    /* fire in the hole !  */
    retcode = node->f_proc( argc, argv );

    dbg1( "%s proc=%p retcode=%d\n", __FUNCTION__, node->f_proc, retcode );

    return retcode;
}

static void debug_dump_node_tree( struct cmd *root, int indent  )
{
    struct cmd *node;
    char prefix[13] = "            "; 

    if( root==NULL ) {
        return;
    }

    XASSERT( indent < 12, indent );
    prefix[indent] = 0;

    node = root;
    while( node ) {
        print_cmd_node( prefix, node );
        debug_dump_node_tree( node->child, indent+2 );
        node = node->next;
    }
}

void cmd_debug_dump( void )
{
    int i;

    for( i=0 ; i<MAX_NUM_CMD ; i++ ) {
        if( base_list[i].namelen ) {
            cmd_printf( "cmd[%d]=\"%s\" len=%d proc=%p\n", i, base_list[i].name,
                    base_list[i].namelen, base_list[i].f_proc);
            debug_dump_node_tree( base_list[i].child, 2 );
        } 
        else {
            cmd_printf( "%d empty\n", i );
        }
    }
}
