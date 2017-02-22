#include "../Kernel/kernel.h"

sem_t s0, s1;
int num=0;

void produtor()
{
    int i;
    for(i=0;i<10000;i++)
    {
       semwait(&s0);
       nkread("%d",&num);
       sempost(&s1);
    }
    taskexit();
}

void consumidor()
{
    int i;
    for(i=0;i<10000;i++)
    {
       semwait(&s1);
       nkprint("num= %d \n",&num);
       sempost(&s0);
    }
    taskexit();
}

int main(int argc, char *argv[])
{
  int t2,t3;
  seminit(&s0, 1);

  seminit (&s1, 0) ;

  taskcreate(&t2,produtor);
  taskcreate(&t3,consumidor);
  start(RR);
  return 0;
}

