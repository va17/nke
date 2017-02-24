#include "syscall.h"

void DoSystemCall(unsigned int *stack,Parameters *arg)
{ 
  Descriptors[TaskRunning].SP=stack;
  MoveToSP(&KernelStack[289]);
#ifdef SHARED_NUMBER
  TTYshared(1);
#endif
  switch(arg->CallNumber)
  {
    case TASKCREATE:
      sys_taskcreate((int *)arg->p0,(void(*)())arg->p1,(unsigned int)(arg->p2),(unsigned int)(arg->p3));
      break;
    case WAITPERIOD:
      sys_waitperiod();
    case SEM_WAIT:
      sys_semwait((sem_t *)arg->p0);
      break;
    case SEM_POST:
      sys_sempost((sem_t *)arg->p0);
      break;
    case SEM_INIT:
      sys_seminit((sem_t *)arg->p0,(int)arg->p1);
      break;
    case WRITELCDN:
      LCDcomando((int)arg->p1);
      LCDnum((int)arg->p0);
      break;
    case WRITELCDS:
      LCDcomando((int)arg->p1);
      LCDputs((char*)arg->p0);
      break;
    case EXITTASK:
      sys_taskexit();
      break;
    case SLEEP:
      sys_sleep((int)arg->p0);
      break;
    case MSLEEP:
      sys_msleep((int)arg->p0);
      break;
    case USLEEP:
      sys_usleep((int)arg->p0);
      break;
    case LIGALED:
      sys_ligaled((int)arg->p0);
      break;
    case START:
      sys_start((int)arg->p0);
      break;
    case TASKJOIN:
      sys_taskjoin((int)arg->p0);
      break;
    case SETMYNAME:
      sys_setmyname((const char *)arg->p0);
      break;
    case NKPRINT:
       sys_nkprint((char *)arg->p0,(void *)arg->p1);
       break;
    case GETMYNUMBER:
       sys_getmynumber((int *)arg->p0);
       break;
    case NKREAD:
       sys_nkread((void *)arg->p0);
       break;
    default:
       break;
  }
  RestoreContext(Descriptors[TaskRunning].SP);
}

void sys_taskcreate(int *ID,void (*task)(),unsigned int Periodo,unsigned int Computacional)
{
  NumberTaskAdd++;
  *ID=NumberTaskAdd;
  Descriptors[NumberTaskAdd].Tid=*ID;
  Descriptors[NumberTaskAdd].EP=task;
  Descriptors[NumberTaskAdd].State=INITIAL;
  Descriptors[NumberTaskAdd].Join=0;
  Descriptors[NumberTaskAdd].Time=0;
  Descriptors[NumberTaskAdd].PrioStatic=0;
  Descriptors[NumberTaskAdd].Prio=0;
  Descriptors[NumberTaskAdd].TimeComputational=Computacional/ClkT;
  Descriptors[NumberTaskAdd].TimeComputationalCounter=Computacional/ClkT;
  Descriptors[NumberTaskAdd].TimePeriodCounter=Descriptors[NumberTaskAdd].TimePeriod=Periodo/ClkT;
  Descriptors[NumberTaskAdd].SP=&Descriptors[NumberTaskAdd].Stack[SizeTaskStack-1];
  return;
}

void sys_waitperiod(void) //Atualizado
{
  Descriptors[TaskRunning].TimeComputationalCounter = 0;
  Descriptors[TaskRunning].TimePeriodCounter = Descriptors[TaskRunning].TimePeriod; 
  Descriptors[TaskRunning].State=BLOCKED;
  //printk("Descriptors[TaskRunning].TimePeriodCounter: ", Descriptors[TaskRunning].TimePeriodCounter);
  Dispatcher();
}
void sys_semwait(sem_t *semaphore)
{
    if(semaphore->count > 0) 			//semaforo aberto?
    {
      semaphore->count = 0;			//bloqueia semaforo
      semaphore->task_id = TaskRunning;
    }else{
      if(Descriptors[TaskRunning].Tid != semaphore->task_id) {
	    semaphore->blocked_number++;
	    semaphore->sem_queue[semaphore->tail] = TaskRunning;
	    Descriptors[TaskRunning].State = BLOCKED ;
	    semaphore->tail++;
	    if(semaphore->tail == MaxNumberTask-1) 
	      semaphore->tail = 0;
	    ordena_sem_queue(semaphore);
	    if(Descriptors[semaphore->task_id].Prio > Descriptors[TaskRunning].Prio)
	      Descriptors[semaphore->task_id].Prio = Descriptors[TaskRunning].Prio;
	    Dispatcher();
      }
    }
#ifdef DEBUG_SEM
      TTYsem(semaphore);
#endif
}

void sys_sempost(sem_t *semaphore)
{

    if(semaphore->blocked_number == 0)
    {
      semaphore->count=1;
      Descriptors[semaphore->task_id].Prio = Descriptors[semaphore->task_id].PrioStatic;
    }else{
      semaphore->blocked_number--;
      Descriptors[semaphore->sem_queue[semaphore->header]].State = READY;
      //restaura prio da task que sai do semaforo
      Descriptors[semaphore->task_id].Prio = Descriptors[semaphore->task_id].PrioStatic; 	
      //recebe nova task no semaforo
      semaphore->task_id = semaphore->sem_queue[semaphore->header];			
      InsertReadyList(semaphore->sem_queue[semaphore->header]);
      semaphore->sem_queue[semaphore->header] = 0;
      if(semaphore->header != semaphore->tail)
      {
	if(semaphore->header == MaxNumberTask-1)
	  semaphore->header = 0;
	else
	  semaphore->header++;
      }
    }
}

void sys_seminit(sem_t *semaphore, int value)
{
    semaphore->count = value;
    semaphore->header = 0;
    semaphore->tail = 0;
    semaphore->task_id = 99;
    semaphore->blocked_number = 0;
}

void sys_taskexit(void)
{
  int i;
  Descriptors[TaskRunning].State=TERMINATED;
  for(i=1;i<=NumberTaskAdd;i++)
  {
    if(TaskRunning==Descriptors[i].Join)
    {
      Descriptors[i].Join=0;
      Descriptors[i].State=READY;
      InsertReadyList(i);
    }
  }
  Dispatcher();
}

void sys_sleep(unsigned int segundo)
{
  Descriptors[TaskRunning].Time = segundo/ClkT;
  if(Descriptors[TaskRunning].Time > 0)
  {
    Descriptors[TaskRunning].State = BLOCKED;
    Dispatcher();
  }
}

void sys_msleep(unsigned int mili)
{
  Descriptors[TaskRunning].Time = (mili/ClkT)/1000;
  if(Descriptors[TaskRunning].Time > 0)
  {
    Descriptors[TaskRunning].State = BLOCKED;
    Dispatcher();
  }
}

void sys_usleep(unsigned int micro)
{
  Descriptors[TaskRunning].Time = (micro/ClkT)/1000000;
  if(Descriptors[TaskRunning].Time > 0)
  {
    Descriptors[TaskRunning].State = BLOCKED;
    Dispatcher();
  }
}

void sys_ligaled(int value)
{
  FIO4PIN=~value;
}

void sys_start(int scheduler)
{
  int i=2,k=0;
  int prioridade_inserida=NumberTaskAdd-1;
  float maior=0;
  SchedulerAlgorithm=scheduler;
  Descriptors[1].State=BLOCKED;
  switch (SchedulerAlgorithm)
  {
    case RR:
    case EDF:
      for(i=2;i<=NumberTaskAdd;i++)
	     InsertReadyList(i);
      break;
    case RM:
      for(i=2;i<=NumberTaskAdd;i++)
      {
	    if(Descriptors[i].TimePeriod>=maior && Descriptors[i].Prio==0)
	    {
	      k=i;
	      maior=Descriptors[i].TimePeriod;
	    }
	    if(i==NumberTaskAdd)
	    {
	    	  InsertReadyList(k);
	    	  Descriptors[k].Prio = prioridade_inserida--;
			  Descriptors[k].PrioStatic = Descriptors[k].Prio;
	    	  if(prioridade_inserida > 0)
	    	  {
				maior=0;
				i=1;
	    	  }
	    }
      }
      break;
    default:
      break;
  }
#ifdef DEBUG_START
  TTYstart();
#endif
  Dispatcher();
}

void sys_taskjoin(int id)
{
  if(Descriptors[id].State!=TERMINATED)
  {
    Descriptors[TaskRunning].State=BLOCKED;
    Descriptors[TaskRunning].Join=id;
    Dispatcher();
  }
}

void sys_setmyname(const char *name)
{
  Descriptors[TaskRunning].name=name;
}

void sys_getmynumber(int *number)
{
  *number=Descriptors[TaskRunning].Tid;
}

void sys_nkread(void *value)
{
  /*	
  serial_queue.tid=TaskRunning;
  serial_queue.value=(int *)value;
  Descriptors[TaskRunning].State=BLOCKED;
  Dispatcher();
  */
}
