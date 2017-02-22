
#include "../Kernel/kernel.h"

int t0,t1;

void task0()
{
  char c;
  while(1)
  {
//    nkprint("\n vou ler um dado da serial: ",0); 
    nkread(&c);
//    nkprint(" %s \n",&c);
//    nkprint(" dado escrito \n",0);
  }
  taskexit();
}

void task1()
{
  while(1)
  {
//    nkprint(" ",0);
  }
  taskexit();
}


int main (int argc, char *argv[])
{
  taskcreate(&t0,task0);
  taskcreate(&t1,task1);
  start(RR);
  return 0;
}