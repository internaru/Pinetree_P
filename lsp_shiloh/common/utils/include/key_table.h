/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef key_table_h
#define key_table_h

/** \file key_table.h
 Generic programming macros to create accessor functions 
 to map 0-n index to string names
 and index or string to object pointers.   

 Returned pointers are NOT owned by caller and should NOT be freed.

 
 define/enum => int => string => pointer to object
 ONLINE_LED => 0 => "online_led" ==> dec_led_t *

 <pre>
 \code
 var_type *   <module>_by_index( unsigned index );
 var_type *   <module>_by_name( const char * str );
 unsigned     <module>_index_by_name( const char * str );
 const char * <module>_name_by_index( unsigned index );
 const char * <module>_name_by_<var_type>( <var_type> * ptr );

 
 OBJECT_t *ptr = MODULE_by_name( "name");
 OBJECT_t *ptr = MODULE_by_index( 1 );
 unsigned index = MODULE_index_by_name( "name" );
 const char *str = MODULE_name_by_index( 1 );
 const char *str = MODULE_name_by_OBJECT_t( ptr );

 program to expand the macro:
 gcc -E key_table_main.c
 begin file key_table_main.c

#include "key_table.h"
typedef signed OBJECT_t; 
KEY_TABLE_DECLARE_PUBLIC_2( MODULE, OBJECT_t );
int main(int argc, char *argv[]){ return 1; }

 end file key_table_main.c
\endcode 
 </pre>
**/




#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

/// Get string from table via index.  @returns null on failure
const char *key_table_name_by_index( unsigned index, const char *table[] );

/// Get object from table via index.  @returns null on failure
void *key_table_obj_by_index( unsigned index, void** table);

/// Get object from table via string.  @returns MAX_INT on failure
unsigned key_table_index_by_name( const char *name, const char *table[] );

/// Get name from table via object pointer.  @returns null on failure
const char * key_table_name_by_objectptr( void *obj, const char *stab[], void** otab);

/// concatination macros never use ## without a macro helper to expand input arguments,
/// concatinates 2 strings
#define CAT_2(a,b) a ## b
/// concatinate 3 strings
#define CAT_3(a,b,c) a ## b ## c
/// concatinate 4 strings
#define CAT_4(a,b,c,d) a ## b ## c ## d
/// concatinate 5 strings
#define CAT_5(a,b,c,d,e) a ## b ## c ## d ## e

/// Provide the key table interface in your header file.
/// module is the module/subsystem/prefix name
/// component is the object/component name 
/// var_type is the object datatype 
/// 
/// Get object my string name or 0-n array index.
/// \code
/// var_type * <module>_by_index( unsigned index ); 
/// var_type * <module>_by_name( const char * str );
///
/// unsigned <module>_index_by_name( const char * str );
/// const char * <module>_name_by_index( unsigned index );
/// \endcode
///
#define KEY_TABLE_DECLARE_PUBLIC_2( module, var_type )                  \
    var_type * CAT_2(module, _by_index)( unsigned index );              \
    const char * CAT_2(module, _name_by_index)( unsigned index );       \
    unsigned CAT_2(module, _index_by_name)( const char * str );         \
    var_type * CAT_2(module, _by_name)( const char * str );             \
    const char * CAT_3(module, _name_by_, var_type)( var_type * obj )

/// same as KEY_TABLE_DECLARE_PUBLIC_2 except it concatenates 
/// \<module\>_\<component\> together for you.
///
#define KEY_TABLE_DECLARE_PUBLIC_3( module, component, var_type )       \
    var_type * CAT_4(module, _, component, _by_index)( unsigned index ); \
    const char * CAT_4(module, _, component, _name_by_index)( unsigned index ); \
    unsigned CAT_4(module, _, component, _index_by_name)( const char * str ); \
    var_type * CAT_4(module, _, component, _by_name)( const char * str ); \
    const char * CAT_5(module, _, component, _name_by_, var_type)( var_type * obj )


/// Construct the keytable implementation in your C file.
/// module is an object name,
/// var_type is the object datatype not the pointer to it.
/// keytable is an array of strings with an extra NULL at the end of the array.
/// objtable is an array of object pointers with an extra null at the end of the array.
///
/// note that the keytable and the objtable are not publicly visible.
///
#define KEY_TABLE_DEFINE_PRIVATE_2( module, var_type, keytable, objtable ) \
    var_type * CAT_2(module, _by_index)( unsigned index ) {             \
        return (var_type *) key_table_obj_by_index( index, (void**) objtable ); \
    }                                                                   \
    const char * CAT_2(module, _name_by_index)( unsigned index ) {      \
        return key_table_name_by_index( index, keytable );              \
    }                                                                   \
    unsigned CAT_2(module, _index_by_name)( const char * name ) {       \
        return key_table_index_by_name( name, keytable );               \
    }                                                                   \
    var_type * CAT_2(module, _by_name)( const char *name ) {            \
        return (var_type *) key_table_obj_by_index(                     \
            key_table_index_by_name( name, keytable ),                  \
            (void**) objtable );                                        \
    }                                                                   \
    const char * CAT_3(module, _name_by_, var_type)( var_type * obj ) { \
        return key_table_name_by_objectptr((void*)obj, keytable, (void**)objtable); \
    }

/// same as KEY_TABLE_DEFINE_PRIVATE_2 except it concatenates 
/// \<module\>_\<component\> together for you.
/// 
#define KEY_TABLE_DEFINE_PRIVATE_3( module, component, var_type, keytable, objtable ) \
    var_type * CAT_4(module, _, component, _by_index)( unsigned index ) { \
        return (var_type *) key_table_obj_by_index( index, (void**) objtable ); \
    }                                                                   \
    const char * CAT_4(module, _, component, _name_by_index)( unsigned index ) { \
        return key_table_name_by_index( index, keytable );              \
    }                                                                   \
    unsigned CAT_4(module, _, component, _index_by_name)( const char * name ) { \
        return key_table_index_by_name( name, keytable );               \
    }                                                                   \
    var_type * CAT_4(module, _, component, _by_name)( const char *name ) { \
        return (var_type *) key_table_obj_by_index(                     \
            key_table_index_by_name( name, keytable ),                  \
            (void**) objtable );                                        \
    }                                                                   \
    const char * CAT_5(module, _, component, _name_by_, var_type)( var_type * obj ) { \
        return key_table_name_by_objectptr((void*)obj, keytable, (void**)objtable); \
    }

/////////////////////////////////////////////////////////////////////////////////////////
#if 0  /* example usage */

// led.h api ...
// unsigned key - string - object mapping, from dec_key_table
typedef struct dec_led_s dec_led_t;

// Declaretion macro or use the example program above to generate the prototype for you 
// then document the get functions generated.
KEY_TABLE_DECLARE_PUBLIC( dec, led, dec_led_t );

// an enum or define is common here
#define ONLINE_LED 0
// it is also common to match the string name to the define/enum
// so ONLINE_LED => 0 => "online_led" ==> dec_led_t *
//

/////////////////////////////////////////////////////////////////////////////////////////
// consumer.c
//
if (0) {
    ASSERT( dec_led_by_name("green_led") == dec_led_by_index(1) );
    ASSERT( dec_led_by_name(dec_led_name_by_index(1)) == dec_led_by_index(1) );
    ASSERT( dec_led_by_index(dec_led_index_by_name("green_led")) == dec_led_by_index(1) );//

    dec_led_on(dec_led_by_index(1));
    dec_led_off(dec_led_by_name("green_led"));
    dec_led_t* green_led = dec_led_by_name("green_led");
    ASSERT(green_led != 0);
    dec_led_on(green_led);
    ASSERT( 0 == dec_led_by_index(11333));
 
}
//////////////////////////////////////////////////////////////////////////////////////////
// led.c private implementation.
//
// array of object pointers with trailing null.
static dec_led_t *myleds[5] = { 
    &my_leds[0], 
    &my_leds[1], 
    &my_leds[2], 
    &my_leds[3],
    0
};

// array of name to match the object pointers with trailing null.
const const char * dec_led_key_table[] = 
{
   "online_led",
    "green_led",
    "red_led",
    "heartbeat_led",
    0
};

// macro expands out to casts on top of key_table_* functions.
KEY_TABLE_DEFINE_PRIVATE_3( dec, led, dec_led_t, dec_led_key_table, myleds );
//------------------------text-text-typedef----array of strings--array of object pointers
//---------------------------------------------const char *[n+1]-dec_led_t *[n+1]

#endif  /* example */

#ifdef __cplusplus
}
#endif

#endif
