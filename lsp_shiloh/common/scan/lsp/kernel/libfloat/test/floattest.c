#include <stdio.h>

int main(int argc, char **argv)
{
  int i;
  float fpnum;
  double dblnum;
  
  fpnum = 1.1;
  for (i=0;i<1000000;i++)
  {
    fpnum = fpnum + 0.0000001;
  }
  printf("fpnum = %f\n",fpnum+2.0f);
}
