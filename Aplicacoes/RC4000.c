#include "../Kernel/kernel.h"
#include<stdlib.h>

sem_t s0,s1,s2;
int num=0;
void pulso()
{
    setmyname(__FUNCTION__);
    while(1);
    taskexit();
}

void Flow_integration()
{
    setmyname(__FUNCTION__);
    while(1);
    taskexit();
}

void Balance_evaluation()
{
    setmyname(__FUNCTION__);
    while(1);
    taskexit();   
}
void data_log_1()
{
    setmyname(__FUNCTION__);
    while(1);
    taskexit();
}
void data_log_2()
{
    setmyname(__FUNCTION__);
    while(1);
    taskexit();
}
void alarm_scanning()
{
    setmyname(__FUNCTION__);
    while(1);
    taskexit();
}
void operator()
{
    setmyname(__FUNCTION__);
    while(1){
	sleep(rand() % 10);
    }
    taskexit();
}

int main(int argc, char *argv[])
{
  seminit(&s0,1);
  int t2,t3,t4,t5,t6,t7,t8;
  taskcreate(&t2,pulso, 1, 0.002);
  taskcreate(&t3,Flow_integration, 2, 0.06);
  taskcreate(&t4,Balance_evaluation, 192, 1);
  taskcreate(&t5,data_log_1, 24, 0.8);
  taskcreate(&t6,data_log_2, 25, 0.6);
  taskcreate(&t7,alarm_scanning, 3, 1);
  taskcreate(&t8,operator, 200, 1);
  start(RM);
  return 0;
}
