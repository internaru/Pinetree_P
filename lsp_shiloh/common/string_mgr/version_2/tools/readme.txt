/******************************************************************************
 * Copyright (c) 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/


To generate new English strings for the system using the configuration file:
string_mgr_config.h use the following command line:

./string_blobber.pl ../config/string_mgr_config.h > ../config/blobEnglish.c


This will use the string_mgr_config.h file as input and redirect the output
of the perl script to "blobEnglish.c".  

NOTE: please make sure that blobEnglish.c is "editable" by your version control
software or that it is not part of your versioned software, but generated
dynamically each build.

