#include "scheduler.h"

void InsertReadyList(int id, int type)
{
  switch(type)
  {
    case PERIODIC:
      if(SchedulerAlgorithm == RR || SchedulerAlgorithm == BG_RR || SchedulerAlgorithm == PS_RR)
      {
        if(ready_queue.queue[ready_queue.head]!=0) //coloca na fila apenas se o começo é diferente do fim
          {
          if(ready_queue.tail<MaxNumberTask-1)
            ready_queue.tail++;
          else
            ready_queue.tail=0;
          }
        ready_queue.queue[ready_queue.tail]=id;
      }
    break;
    case APERIODIC:
      if(ap_ready_queue.queue[ap_ready_queue.head]!=0) //coloca na fila apenas se o começo é diferente do fim
        {
        if(ap_ready_queue.tail<MaxNumberTask-1)
          ap_ready_queue.tail++;
        else
          ap_ready_queue.tail=0;
        }
      ap_ready_queue.queue[ap_ready_queue.tail]=id;
    break;
    default:
    break;
  }
}

void WakeUP()
{
  int i=2;
  for(i=2;i<=NumberTaskAdd; i++)
  {
    //sleep
    if(Descriptors[i].Time>0)
    {
      Descriptors[i].Time--;
      if(Descriptors[i].Time <= 0 && Descriptors[i].State == BLOCKED)
      {
        Descriptors[i].State = READY;
        InsertReadyList(i, Descriptors[i].Type);
      }
    }
  }
}

void Dispatcher(void) //responsavel por realizar o disparo/restauração da tarefa
{
    
  switch(Descriptors[TaskRunning].State)
  {
    case TERMINATED:
    case BLOCKED:
    case READY:
      TaskRunning=ready_queue.queue[ready_queue.head];
      ready_queue.queue[ready_queue.head]=0;
      if(ready_queue.head!=ready_queue.tail)
      {
      if(ready_queue.head<MaxNumberTask-1)
        ready_queue.head++;
      else
        ready_queue.head=0;
      }
      if(Descriptors[TaskRunning].State!=READY)
       Dispatcher();
      else
      {
#ifdef DEBUG_SYSTEM
      TTYout();
#endif
#ifdef DEBUG_PLOT
      TTYplot();
#endif
#ifdef SHARED_NUMBER
      TTYshared(0);
#endif
      T0IR = 1;
      VICVectAddr = 0;
      Descriptors[TaskRunning].State=RUNNING;
      RestoreContext(Descriptors[TaskRunning].SP);
      }
      break;
    case INITIAL:
#ifdef DEBUG_SYSTEM
      TTYout();
#endif
#ifdef DEBUG_PLOT
      TTYplot();
#endif
#ifdef SHARED_NUMBER
      TTYshared(0);
#endif
MoveToSP(&KernelStack[299]);
      T0IR = 1;
      VICVectAddr = 0;
      Descriptors[TaskRunning].State=RUNNING;
      GoTask(Descriptors[TaskRunning].SP,Descriptors[TaskRunning].EP);
      break;
    default:
#ifdef DEBUG_SYSTEM
      TTYout();
#endif
#ifdef DEBUG_PLOT
      TTYplot();
#endif
#ifdef SHARED_NUMBER
      TTYshared(0);
#endif
//MoveToSP(&KernelStack[299]);
      T0IR = 1;
      VICVectAddr = 0;
      Descriptors[TaskRunning].State=RUNNING;
      RestoreContext(Descriptors[TaskRunning].SP);
      break;
  }

}

void Select(unsigned int *pilha)
{
  Descriptors[TaskRunning].SP=pilha; //salva a pilha do contexto da task
  MoveToSP(&KernelStack[289]);
  WakeUP();
  U0getchar();
  switch(SchedulerAlgorithm)
  {
    case RR:
      if(TaskRunning!=1)
        Descriptors[TaskRunning].State=READY;
      if(TaskRunning>1)
        InsertReadyList(TaskRunning, Descriptors[TaskRunning].Type);
      break;
    case BG_RR:
      // implementar Background aqui
      break;
    case PS_RR:
      // implementar Polling Server aqui
      break;
    default:
      break;
  }
  Dispatcher();  
}
