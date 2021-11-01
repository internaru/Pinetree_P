/* 
 *
 * ============================================================================
 * Copyright (c) 2012  Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#include "http_oid_lut_config.h"
#include "http_parse.h"
#include "lassert.h"
#include "memAPI.h"

void updateOidLink(HttpPageContext* context, char* value)
{
    ASSERT(NULL != context);
    ASSERT(NULL != value);

    oidLink *oid_link = http_oid_lut_get_last_oid(context);
    if(NULL == oid_link)
    {
        return;
    }

    // Add custom code here

}

/** 
 * \brief Create oid links that contain disabled product
 * features
 * 
 * \param context - context for links
 * 
 * \param oid - indicates which oids need to be created 
 *  
 * \param index - index for oid to create 
 * 
 * \return bool - indicates success of function
 * 
 * Some protocols specify that all features are disabled except
 * those that are specifically enabled.  To accomplish this,
 * when certain start tags are parsed, the oids that correspond
 * to set of subsequent tag values are created in the disabled
 * state.  Then as the enabled features are parsed, the oid
 * links are updated.
 * 
 * For example - when we parse the dd:ApplicationsEnabled tag -
 * we create oid links to disable mDNS, DHCP, SLP... and then
 * re-enable these features if they are listed in the XML that
 * follows the initial dd:ApplicationsEnabled tag.
 * 
 **/
bool createOidLinks( HttpPageContext* context, oid_t oid, uint32_t index )
{
    bool retVal = false;

    switch ( oid )
    {
        default:
            if ( index == DYNAMIC_INDEX_SELECTION )
            {
                oidLink *last_oid;
                last_oid = http_oid_lut_get_last_oid( context );
                if ( last_oid->oid == oid )
                {
                    index = last_oid->index + 1;
                }
                else
                {
                    index = 0;
                }
            }

            retVal = addOidLinkToContext( context, 
                                          oid,
                                          index,
                                          NULL );
            break;
    }

    return retVal;

}
