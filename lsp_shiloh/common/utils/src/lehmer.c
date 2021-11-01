/* simple Lehmer pseudorandom number generator from
 *  Copyright (c)  1990 _Algorithms in C_, Sedgwick
 */

#define m 100000000
#define m1 10000
#define b 31415821

/* XXX not thread safe! */
static unsigned int rseed;

static unsigned int lehmer_mult( int p, int q )
{
    unsigned int p1, p0, q1, q0;
    p1 = p/m1; p0=p%m1;
    q1 = q/m1; q0=q%m1;
    return (((p0*q1+p1*q0) % m1)*m1+p0*q0) %m;
}

unsigned int lehmer_random( void )
{
    rseed = (lehmer_mult(rseed,b)+1) % m;
//    dbg1(( "rseed=%d %#x\n",rseed, rseed));
    return rseed;
}

double lehmer_frandom( void )
{
    return (lehmer_random() * 1.0) / m;
}

void lehmer_srandom( unsigned int seed )
{
    rseed = seed;
}

