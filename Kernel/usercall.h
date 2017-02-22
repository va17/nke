
#ifndef USERCALL_H_
#define USERCALL_H_

#include "syscall.h"

void taskcreate(int *ID,void (*funcao)());
void semwait(sem_t *semaforo);
void sempost(sem_t *semaforo);
void seminit(sem_t *semaforo, int ValorInicial);
void writelcdn(int msg,int pos);
void writelcds(char* msg,int pos);
void taskexit(void);
void sleep(int time);
void msleep(int time);
void usleep(int time);
void ligaLed(int valor);
void start(int scheduler);
void taskjoin(int identificador);
void setmyname(const char *name);
void nkprint(char *fmt,void *number);
void getmynumber(int *number);
void nkread(char *tipo,void *value);

#endif /* USERCALL_H_ */
