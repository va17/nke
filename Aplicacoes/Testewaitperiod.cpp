#include "../Kernel/kernel.h"
int aux=0, buffer = 0 ;
sem_t s0,s1;
void task0()
{
    int i;
    for(i=0;i<1000;i++){
      waitperiod();
      semwait(&s0);                
	  buffer+=1;
	  sempost(&s1);
    }
    taskexit();
}

void task1()
{
    int i;
    for(i=0;i<1000;i++){
      waitperiod();
      semwait(&s1)
      aux = buffer ;
	  sempost(&s0);
    }
    nkprint("Valor de aux = %d\n",aux);
    taskexit();
} 


int main(int argc, char *argv[])
{
  int t1,t2;
  seminit(&s0,1);
  seminit(&s1,0);
  taskcreate(&t2,task0, 10, 2);
  taskcreate(&t3,task1, 15, 4);
  
  start(RM);
  return 0;
}
