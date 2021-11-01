/* davep 08-Nov-2005 ; simple Lehmer pseudorandom number generator from
 * _Algorithms in C_, Sedgwick copyright 1990
 */
 
#ifndef LEHMER_H
#define LEHMER_H

void lehmer_srandom( unsigned int seed );
unsigned int lehmer_random( void );
double lehmer_frandom( void );

#endif

