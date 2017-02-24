#include "../Kernel/kernel.h"
int aux=0;
sem_t s1,s2,s3;
void task0()
{
    int i;
    for(i=0;i<1000;i++){
      semwait(&s1);
	  aux+=2;
      sempost(&s1); 
    }
      sempost(&s2); 
    taskexit();
}

void task1()
{
    int i;
    for(i=0;i<1000;i++){
      semwait(&s1);
	aux+=3;
      sempost(&s1); 
    }
      sempost(&s3); 
    taskexit();
} 

void task2()
{
    semwait(&s2); 
    semwait(&s3); 
    nkprint("%d ",aux);
    taskexit();   
}

int main(int argc, char *argv[])
{
  int t4,t2,t3,t5, t6, t7, t8, t9, t10, t11;
  seminit(&s1,5);
  seminit(&s2,-10);
  seminit(&s3,0);
  taskcreate(&t2,task0, 10, 2);
  taskcreate(&t3,task1, 15, 4);
  taskcreate(&t4,task2, 25, 10);
  
start(RM);
  return 0;
}
