
#include "../Kernel/kernel.h"
int t[MaxNumberTask-1];

void teste1()
{
  int id;
  getmynumber(&id);
  float c;

  while(1){
    nkread("%f",&c); 
    nkprint("Task %d recebeu: ",&id);
    nkprint("%f\n", &c); 
    } 
   taskexit();
}

void teste2()
{
  int id;
  getmynumber(&id);
  int e;
  while(1){
    nkread("%d",&e);  
    nkprint("Task %d recebeu: ",&id);
    nkprint("%d\n", &e);
    }
  taskexit();
}

void teste3()
{
  int id;
  getmynumber(&id);
  char f;
  while(1){
    nkread("%c",&f);  
    nkprint("Task %d recebeu: ",&id);
    nkprint("%c\n", &f);
    }
  taskexit();
}

void teste4()
{
  int id;
  getmynumber(&id);
  char d;
  while(1){
    nkread("%s",&d);  
    nkprint("Task %d recebeu: ",&id);
    nkprint("%s\n", &d);
    }
  taskexit();
}
int main (int argc, char *argv[])
{
  int i;
  i=0;
		 taskcreate(&t[i],teste1); i++;
           nkprint("\nTask %d criada [real]... \n",&i); 
		// taskcreate(&t[i],teste2); i++;
         //  nkprint("Task %d criada [inteiro]... \n",&i); 
		// taskcreate(&t[i],teste3); i++;	
          // nkprint("Task %d criada [caracter]... \n",&i);	
		// taskcreate(&t[i],teste4); i++;
         //  nkprint("Task %d criada [string]... \n",&i);		
	
  start(RR);
  return 0;
}
