#include "../Kernel/kernel.h"

sem_t semaforo,s1,s2;
void task0()
{
    setmyname(__FUNCTION__);
    sleep(5);
    while(1){
    semwait(&semaforo);
 //   sempost(&semaforo);
    }
    taskexit();
}

void task1()
{
    int num;
    setmyname(__FUNCTION__);
    while(1){
    semwait(&s1);
//	sleep(5);

    getmynumber(&num);
    nkprint("Sou a task %d\n",num);
    }
    taskexit();
}

void task2()
{
    int num2;
    setmyname(__FUNCTION__);
    //semwait(&semaforo);
    while(1){
    getmynumber(&num2);
    sleep(5);
    sempost(&s1);
	}
    taskexit();   
}

int main(int argc, char *argv[])
{
  int t4,t2,t3,t5, t6, t7, t8, t9, t10, t11;
  seminit(&s1,0);
//  taskcreate(&t2,task0, 10, 2);
  taskcreate(&t3,task1, 15, 4);
  taskcreate(&t4,task2, 25, 10);
  taskcreate(&t5,task1, 15, 4);
  taskcreate(&t6,task1, 15, 4);
  taskcreate(&t7,task1, 15, 4);
  taskcreate(&t8,task1, 15, 4);
  taskcreate(&t9,task1, 15, 4);
  
start(RR);
  return 0;
}
