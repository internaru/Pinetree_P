/*
 **************************************************************************************
 *
 * Copyright (c) 2012 Marvell International, Ltd.
 *
 **************************************************************************************
 *
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell as part of a proprietary software release,
 * the File is considered Marvell Proprietary and Confidential Information, and is
 * licensed to you under the terms of the applicable Commercial License.
 *
 **************************************************************************************
 *
 * Marvell GPL License Option
 *
 * If you received this File from Marvell as part of a Linux distribution, this File
 * is licensed to you in accordance with the terms and conditions of the General Public
 * License Version 2, June 1991 (the "GPL License").  You can redistribute it and/or
 * modify it under the terms of the GPL License; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GPL License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program.  If not, see http://www.gnu.org/licenses/.
 *
 **************************************************************************************
*/

/*
 * Clone-ish of common OBA code utils.h
 */

#ifndef UTILS_H
#define UTILS_H

#ifndef MIN
#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#endif

#ifndef MAX
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )
#endif

int str_mkint( const char *str, uint32_t *num32 );
bool str_match( const char *s1, const char *s2, int chars_to_match ) ;
int str_split( char *src, int srclen, char *tokens[], int tokenslen, const char *delims );
void str_tr( char *str, const char *from, const char *to, int len );
char * trim(char * string);

#endif

