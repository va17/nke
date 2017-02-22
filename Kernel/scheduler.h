#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "kernel.h"
#include "../Placa/lcd.h"
#include "../Placa/interrupt.h"
#include "syscall.h"
#include "initkernel.h"
#include "debug.h"
#include <string.h>

enum Scheduler{
  RR,    // 0
  RM,    // 1
  EDF,   // 2
  BG_RR, // 3
  PS_RR  // 4
};

void Select(unsigned int *pilha);
void InsertReadyList(int id);
void SetTime(void);
void Dispatcher(void);
void UseKernelStack(void);
void WakeUP(void);
void DoNets(void);

#endif
