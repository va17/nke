#include "scheduler.h"

void InsertReadyList(int id)
{
  unsigned short tail_aux,tail_aux2;
  unsigned short head_aux;
  short atualiza_tail=0;
  int i;
  switch(SchedulerAlgorithm)
  {
    case RR:
      if(ready_queue.queue[ready_queue.head]!=0) //coloca na fila apenas se o começo é diferente do fim
      {
    	if(ready_queue.tail<MaxNumberTask-1)
    		ready_queue.tail++;
    	else
    		ready_queue.tail=0;
      }
      ready_queue.queue[ready_queue.tail]=id;
      break;
    case RM:
      if(ready_queue.queue[ready_queue.head]==0)
    	  ready_queue.queue[ready_queue.head]=id;
      else
      {
    	tail_aux=ready_queue.tail;
    	while(1)
    	{
	      if(Descriptors[id].Prio < Descriptors[ready_queue.queue[tail_aux]].Prio) //se o numero inserido for menor que o tail
	      {
	        tail_aux2=tail_aux; //guarda o valor
	        if(tail_aux2==MaxNumberTask-1) //avanca o tail
	          tail_aux2=0;
	        else
	          tail_aux2++;
	        ready_queue.queue[tail_aux2]=ready_queue.queue[tail_aux]; //copia o valor anterior
	        if(atualiza_tail==0)
	        {
	          atualiza_tail=1;
	          ready_queue.tail=tail_aux2;
	        }
	        if(tail_aux==0)
	          tail_aux=MaxNumberTask-1;
	        else
	          tail_aux--;
	      }
	      else
	      {
	        if(tail_aux==MaxNumberTask-1) //avanca o tail
	          tail_aux=0;
	        else
	          tail_aux++;
	        if(atualiza_tail==0)
	        {
	          atualiza_tail=1;
	          ready_queue.tail=tail_aux;
	        }
	        ready_queue.queue[tail_aux]=id;
	        break;
	      }
    	}
      }
      break;
    case EDF:
      if(ready_queue.queue[ready_queue.head]==0)
    	  ready_queue.queue[ready_queue.head]=id;
      else
      {
	      tail_aux=ready_queue.tail;
	      while(1)
	      {
	        if(Descriptors[id].TimePeriodCounter<Descriptors[ready_queue.queue[tail_aux]].TimePeriodCounter) //se o numero inserido for menor que o tail
	        {
	          tail_aux2=tail_aux; //guarda o valor
	          if(tail_aux2==MaxNumberTask-1) //avanca o tail
	            tail_aux2=0;
	          else
	            tail_aux2++;
	          ready_queue.queue[tail_aux2]=ready_queue.queue[tail_aux]; //copia o valor anterior
	          if(atualiza_tail==0)
	          {
	            atualiza_tail=1;
	            ready_queue.tail=tail_aux2;
	          }
	          if(tail_aux==0)
	            tail_aux=MaxNumberTask-1;
	          else
	            tail_aux--;
	        }
	        else
	        {
	          if(tail_aux==MaxNumberTask-1) //avanca o tail
	            tail_aux=0;
	          else
	            tail_aux++;
	          if(atualiza_tail==0)
	          {
	            atualiza_tail=1;
	            ready_queue.tail=tail_aux;
	          }
	          ready_queue.queue[tail_aux]=id;
	          break;
	        }
	      }
      }
      head_aux=ready_queue.head;
      i=1;
      do
      {
	      Descriptors[ready_queue.queue[head_aux]].Prio=i;
	      i++;
	      if(head_aux==MaxNumberTask-1)
	        head_aux=0;
	      else
	        head_aux++;
      }while(head_aux!=ready_queue.tail);
      Descriptors[ready_queue.queue[head_aux]].Prio=i;
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
        InsertReadyList(i); 
	    if((SchedulerAlgorithm == RM || SchedulerAlgorithm == EDF) && Descriptors[TaskRunning].State == RUNNING) {
              Descriptors[TaskRunning].State = READY;
              if(TaskRunning!=0)
                  InsertReadyList(TaskRunning);
        }
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
  int i,oldtaskrunning;
  Descriptors[TaskRunning].SP=pilha; //salva a pilha do contexto da task
  MoveToSP(&KernelStack[289]);
  WakeUP();
  U0getchar();
  switch (SchedulerAlgorithm)
  {
    case RR:
      if(TaskRunning!=1)
	      Descriptors[TaskRunning].State=READY;
      if(TaskRunning>1)
    	  InsertReadyList(TaskRunning);
      break;
    case RM:
      if(TaskRunning!=0)
      {
	      if(Descriptors[TaskRunning].TimeComputationalCounter>0)
	        Descriptors[TaskRunning].TimeComputationalCounter--;
	      if(Descriptors[TaskRunning].TimeComputationalCounter<=0)
	        Descriptors[TaskRunning].State=BLOCKED;
      }
      for(i=2;i<=NumberTaskAdd; i++)
      {
	      if(Descriptors[i].TimePeriodCounter>0)
	        Descriptors[i].TimePeriodCounter--;
      	if(Descriptors[i].TimePeriodCounter<=0)
	      {
	        if(!(Descriptors[i].TimeComputationalCounter>0))
	        {
	          Descriptors[i].TimePeriodCounter=Descriptors[i].TimePeriod;
	          Descriptors[i].TimeComputationalCounter=Descriptors[i].TimeComputational;
	          Descriptors[i].State = READY;
	          InsertReadyList(i); //deadline a tratar
	          if(TaskRunning!=i && Descriptors[TaskRunning].State==RUNNING) //se a tarefa que acabou o tempo Computacional estava executando, ela não voltará duas vezes
	          {
	            Descriptors[TaskRunning].State = READY;
	            if(TaskRunning!=0)
		          InsertReadyList(TaskRunning);
	          }
	        }
	        else //erro deadline
	        {
	          Descriptors[i].Deadline=1;
	         //TTYout adicionar perda de deadline
	        }
	      }
      }
      break;
    case EDF:
      if(TaskRunning!=0)
      {
	      if(Descriptors[TaskRunning].TimeComputationalCounter>0)
	        Descriptors[TaskRunning].TimeComputationalCounter--;
	      if(Descriptors[TaskRunning].TimeComputationalCounter<=0)
	        Descriptors[TaskRunning].State=BLOCKED;
      }
      oldtaskrunning=TaskRunning;
      for(i=2;i<=NumberTaskAdd; i++)
      {
	      if(Descriptors[i].TimePeriodCounter>0)
	        Descriptors[i].TimePeriodCounter--;
	      if(Descriptors[i].TimePeriodCounter<=0)
	      {
	        if(!(Descriptors[i].TimeComputationalCounter>0))
	        {
	          Descriptors[i].TimePeriodCounter=Descriptors[i].TimePeriod;
	          Descriptors[i].TimeComputationalCounter=Descriptors[i].TimeComputational;
	          Descriptors[i].State = READY;
	          InsertReadyList(i); //colocar um flag (só pode adicionar a old depois das atualizações !!!!)
	          if(oldtaskrunning!=i && Descriptors[oldtaskrunning].State==RUNNING)
	          {
	            Descriptors[oldtaskrunning].State = READY;
	            if(oldtaskrunning!=0)
		            InsertReadyList(oldtaskrunning);
	          }
	        }
	        else //erro deadline
	        {
	          Descriptors[i].Deadline=1;
	          //TTYout adicionar perda de deadline
	        }
	      }
      }
      break;
    default:
      break;
  }
  Dispatcher();  
}
