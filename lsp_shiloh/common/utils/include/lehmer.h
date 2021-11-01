/* simple Lehmer pseudorandom number generator from
 *  Copyright (c)  1990 _Algorithms in C_, Sedgwick
 */
 
#ifndef LEHMER_H
#define LEHMER_H

void lehmer_srandom( unsigned int seed );
unsigned int lehmer_random( void );
double lehmer_frandom( void );

#endif

