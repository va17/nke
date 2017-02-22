#include "../Kernel/kernel.h"
sem_t semaforo,s0,s1,s2,s3,s4,s5;
int buffer;
void tarefap0()
{
 int i;
  while(1)
  {
   i = rand(100);
   semwait(&s0);
   buffer = i;
   sempost(&s1);
  }
  while(1);
}

void tarefat0()
{
 int i;
  while(1){
   semwait(&s1);
   i = buffer;
	    nkprint("Task 0 gerou %d\n",i);
   sempost(&s0);
  }
}

void tarefap1()
{
 int i;
  while(1)
  {
   i = rand(100);
   semwait(&s2);
   buffer = i;
   sempost(&s3);
  }
  while(1);
}

void tarefat1()
{
 int i;
  while(1){
   semwait(&s3);
   i = buffer;
            nkprint("Task 1 gerou %d\n",i);
   sempost(&s2);
  }
}

void tarefap2()
{
 int i;
  while(1)
  {
   i = rand(100);
   semwait(&s4);
   buffer = i;
   sempost(&s5);
  }
  while(1);
}

void tarefat2()
{
 int i;
  while(1){
   semwait(&s5);
   i = buffer;
	     nkprint("Task 2 gerou %d\n",i);
   sempost(&s4);
  }
}

int main(int argc, char *argv[])
{
  int tp0,tt0,tp1,tt1,tp2,tt2;
  seminit(&s0,1);
  seminit(&s1,0);
  seminit(&s2,1);
  seminit(&s3,0);
  seminit(&s4,1);
  seminit(&s5,0);
  taskcreate(&tp0,tarefap0,1000,20);
  taskcreate(&tt0,tarefat0,1000,20);
  taskcreate(&tp1,tarefap1,500,35);
  taskcreate(&tt1,tarefat1,500,35);
  taskcreate(&tp2,tarefap2,200,10);
  taskcreate(&tt2,tarefat2,200,10);
  start(RM);

  return 0;
}
