//#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
   long ll_foo, ll_bar;
   double    d_foo;
  
   ll_foo = 56;
   d_foo = (double)ll_foo;
   ll_bar = (long long)d_foo;   

   printf("ll_foo = %ld\n",ll_foo+56L);
   printf("ll_bar = %ld\n",ll_bar);
}
