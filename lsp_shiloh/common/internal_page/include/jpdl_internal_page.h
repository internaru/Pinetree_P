/***********************************************************
* (c) Copyright 2008 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

#ifndef __JPDL_INTERNAL_PAGE_H__
#define __JPDL_INTERNAL_PAGE_H__


#if defined(HAVE_KINOMA) && defined(HAVE_JPDL_INTERNAL_PAGES)
// pageName can be just the basename of a .jpdl file, or it can be the full
// path to any file on the file system. If only the file's name is given, it
// is expected that we can find it in the intpage subdirectory of the Kinoma
// docs directory.
void printJpdlPage( const char *pageName );
#endif // HAVE_KINOMA

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __JPDL_INTERNAL_PAGE_H__

