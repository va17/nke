
#include "../Kernel/kernel.h"

void tarefa1()
{
  nkprint("inicio ",0);
  int a;
  a=0;
  while(1)
  {
    nkprint("a=%d, ",a++);
  }
  while(1);
}

void tarefa2()
{
  nkprint("inicio ",0);
  while(1){
    nkprint("t2 ",0);
  }
}

void tarefa3()
{
nkprint("inicio ",0);
  while(1){
    nkprint("t3 ",0);
  }
}

int main(int argc, char *argv[])
{
  int t1,t2,t3;
  taskcreate(&t1,tarefa1,10,2);
  taskcreate(&t2,tarefa2,15,4);
  taskcreate(&t3,tarefa3,35,10);
  start(RM);

  return 0;
}
