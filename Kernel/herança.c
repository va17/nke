#include "herança.h"
void ordena_sem_queue(sem_t* sem)
{
	int i, j, aux_id, aux_prio, temp_prio[MaxNumberTask], temp_id[MaxNumberTask];
	for(i = sem->header; i < sem->tail; i++){
		temp_prio[i] = Descriptors[sem->sem_queue[i]].Prio;
		if(i == MaxNumberTask-1)
			i = 0;
	}
	for(i = sem->header; i < sem->tail; i++){
		aux_id = MaxNumberTask;
		aux_prio = MaxNumberTask;
		for(j = sem->header; j < sem->tail; j++){
		    	if(Descriptors[sem->sem_queue[j]].Prio < aux_prio){
				if(i != sem->header){
					if(Descriptors[sem->sem_queue[j]].Prio > temp_prio[i-1]){
						aux_id = sem->sem_queue[j];
						aux_prio = Descriptors[sem->sem_queue[j]].Prio;
					}
				} else {
					aux_id = sem->sem_queue[j];
					aux_prio = Descriptors[sem->sem_queue[j]].Prio;
				}
			}
			if(j == MaxNumberTask-1)
				j = 0;
		}
		temp_prio[i] = aux_prio;
		temp_id[i] = aux_id;
		if(i == MaxNumberTask-1)
			i = 0;
	}
	for(i = sem->header; i < sem->tail; i++){
		sem->sem_queue[i] = temp_id[i];
		if(i == MaxNumberTask-1)
			i = 0;
	}
}

