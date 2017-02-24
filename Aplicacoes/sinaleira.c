#include "../Kernel/kernel.h"
#include<stdlib.h>

sem_t semaforo,s1,s2;
static int num=0, carros_ld=0, carros_le=0, ocupada=0;


void status(){
	if(ocupada>0){
		nkprint("\n>>>>> >>>>> >>>>> Ponte ocupada: >>>>> >>>>> >>>>>",0);
	}else if(ocupada<0){
		nkprint("\n<<<<< <<<<< <<<<< Ponte ocupada: <<<<< <<<<< <<<<<",0);
	}else{
		nkprint("\n_________________ Ponte livre ____________________",0);
	}
	nkprint("\n %d carros no lado direito.",carros_ld);
	nkprint(" %d carros no lado esquerdo.\n",carros_le);
}


void Chega_carro_ld()
{
    while(1){
	sleep(rand() % 30);
	carros_ld++;
	nkprint("+",0);
	status();
    }
    taskexit();
}
void Chega_carro_le()
{
    while(1){
	sleep(rand() % 30);
	carros_le++;
	nkprint("+",0);
	status();
    }
    taskexit();
}
void Fluxo_ld(){
    while(1){
	nkprint("",0);
	if(carros_ld>0){
		semwait(&semaforo);
		ocupada=1;		
		nkprint("\nLuz verde para o lado direito.",0);
		while(carros_ld!=0){
			status();
			sleep(5);
			carros_ld--;
			nkprint("-",0);
		}
		ocupada=0;
		nkprint("\nLuz vermelha para o lado direito.",0);
		status();
		sempost(&semaforo);
	}
    }
    taskexit();
}
void Fluxo_le(){
    while(1){
	nkprint("",0);
	if(carros_le>0){
		semwait(&semaforo);
		ocupada=-1;
		nkprint("\nLuz verde para o lado esquerdo.",0);
		while(carros_le!=0){
			status();
			sleep(5);
			carros_le--;
			nkprint("-",0);
		}
		ocupada=0;
		nkprint("\nLuz vermelha para o lado esquerda.",0);
		status();
		sempost(&semaforo);
	}
    }
    taskexit();
}

int main(int argc, char *argv[])
{
  seminit(&semaforo,1);
  int t2,t3,t4,t5;
  taskcreate(&t2,Chega_carro_ld, 5, 25);
  taskcreate(&t3,Chega_carro_le, 6, 36);
  taskcreate(&t4,Fluxo_ld, 5, 10);
  taskcreate(&t5,Fluxo_le, 16, 50);
  start(RR);
  return 0;
}
