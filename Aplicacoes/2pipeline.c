#include "../Kernel/kernel.h"

sem_t s0,s1,s2;
int num=0;

void task0()
{
    int i;
    while(1)
    {
       semwait(&s0);
       nkprint(" num = %d",&num);
       sempost(&s0);
    }
    taskexit();
}

void task1()
{
    int i;
    for(i=0;i<10;i++)
    {
       semwait(&s0);
       nkread(&num);
       sempost(&s0);
    }
    taskexit();
}

void task2()
{
    int i;
    for(i=0;i<10;i++)
    {
       semwait(&s2);
       num++;
       nkprint("num = %d\n", num);
       sempost(&s0);
    }
    taskexit();
}

int main(int argc, char *argv[])
{
  seminit(&s0,1);
  int t2,t3;
  taskcreate(&t2,task0, 10, 2);
  taskcreate(&t3,task1, 15, 4);
//  taskcreate(&t4,task2, 35, 10);
  start(RR);
  return 0;
}
