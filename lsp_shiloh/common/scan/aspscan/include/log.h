/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef LOG_H
#define LOG_H

#define dbg1(...) debuglog( 1, __VA_ARGS__ )
#define dbg2(...) debuglog( 2, __VA_ARGS__ )
#define dbg3(...) debuglog( 3, __VA_ARGS__ )

void debuglog( int level, const char *fmt, ... ) __attribute__ ((format (printf,2,3))); 
void errorlog( const char *fmt, ... ) __attribute__ ((format (printf,1,2)));

int log_set_level( int new_log_level );

#endif /* LOG_H */

