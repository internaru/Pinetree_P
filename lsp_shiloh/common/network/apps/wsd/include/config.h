/*============================================================================*\
|                                                                              |
|                      SOA4D DPWSCore (C DPWS toolkit)                         |
|                                                                              |
|                                                                              |
| This file belongs to the SOA4D DPWSCore distribution.                        |
| Copyright (C) 2004-2013 Schneider Electric.                                  |
| All rights reserved.                                                         |
|                                                                              |
| Redistribution and use in source and binary forms, with or without           |
| modification, are permitted provided that the following conditions are met:  |
|                                                                              |
| 1. Redistributions of source code must retain the above copyright notice,    |
| this list of conditions and the following disclaimer.                        |
|                                                                              |
| 2. Redistributions in binary form must reproduce the above copyright notice, |
| this list of conditions and the following disclaimer in the documentation    |
| and/or other materials provided with the distribution.                       |
|                                                                              |
| 3. Neither the name of Schneider Electric nor the names of its contributors  |
| may be used to endorse or promote products derived from this software        |
| without specific prior written permission.                                   |
|                                                                              |
| THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS          |
| "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED    |
| TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR   |
| PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR            |
| CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,        |
| EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,          |
| PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;  |
| OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,     |
| WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR      |
| OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF       |
| ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                   |
|                                                                              |
| For more information on this software, see http://www.soa4d.org.             |
|                                                                              |
|       + File info:                                                           |
|                     $Revision: 1062 $
|                     $Date: 2008-05-05 17:50:06 +0200 (lun., 05 mai 2008) $
\*============================================================================*/
/*******************************************************************************
*                         Target specific definitions                          *
*******************************************************************************/
#ifndef CONFIG_H_
#define CONFIG_H_


#include <sys/socket.h>	// removed by WITH_NOIO in dcGSOAP_Runtime.h
#include <netinet/in.h>	// removed by WITH_NOIO in dcGSOAP_Runtime.h

#define HAVE_STRTOD
#define HAVE_SSCANF
#define HAVE_STRTOL
#define HAVE_STRTOUL
#define HAVE_STRTOLL
#define HAVE_STRTOULL
#define HAVE_SYS_TIMEB_H
#define HAVE_FTIME
#define HAVE_RAND_R
#define HAVE_GMTIME_R
#define HAVE_LOCALTIME_R
#define HAVE_TIMEGM
#define HAVE_WCTOMB
#define HAVE_MBTOWC
#define HAVE_ISNAN


#endif /*CONFIG_H_*/
