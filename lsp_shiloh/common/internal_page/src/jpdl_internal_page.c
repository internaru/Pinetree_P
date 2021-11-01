/****************************************************************************** 
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#if defined(HAVE_KINOMA) && defined(HAVE_JPDL_INTERNAL_PAGES)
// pageName can be just the basename of a .jpdl file, or it can be the full
// path to any file on the file system. If only the file's name is given, it
// is expected that we can find it in the intpage subdirectory of the Kinoma
// docs directory.

void printJpdlPage( const char *pageName )
{
    void printInternalJpdlPage( const char *pageName );
    
    if ( pageName )
    {
        printInternalJpdlPage( pageName );
    }
}

#endif // defined(HAVE_KINOMA) && defined(HAVE_JPDL_INTERNAL_PAGES)


