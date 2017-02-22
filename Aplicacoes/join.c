
#include "../Kernel/kernel.h"

int t0,t1;

void task0()
{
  int i;
  nkprint("\n vou espera a tarefa %d acabar",t1); 
  taskjoin(t1);
  nkprint("\n acordei!! sou a tarefa %d",t0);
  taskexit();
}

void task1()
{ 
  int i;
  for(i=0;i<10;i++)
  {
    nkprint("\n buffer = %d",i);
  }
  nkprint("\n sou a tarefa %d e vou morrer",t1);
  taskexit();
}

int main (int argc, char *argv[])
{
  taskcreate(&t0,task0);
  taskcreate(&t1,task1);
  start(RR);
  return 0;
}
