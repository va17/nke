#ifndef __INITKERNEL_H
#define __INITKERNEL_H

/* Task types */
#define PERIODIC 0
#define APERIODIC 1

#include "kernel.h"
#include "../Placa/interrupt.h"
#include "../Placa/init.h"
#include "../Placa/lcd.h"

void StartKernel(void);
void StartReadyList(void);
void ini_serial_fila(void);
void CreateIdleAndMain(void);


#endif
