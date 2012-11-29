#include "type.h"

PROC proc[NPROC];
PROC *freeList, *running, *readyQueue;
int procSize = sizeof(PROC);

/******************** use your OWN LAB5 code *****************
 #include "io.c"
 #include "queue.c"
 #include "wait.c"   
 #include "fork_exec.c"
*************************************************************/


#include "queue.c"      // queue functions 
#include "fork_exec.c"  // kfork, do_kfork, kexec
#include "wait.c"       // kwait, kexit
#include "int.c"        
#include "pv.c"
#include "serial.c"     // functions for serial port driver 


int body();

char *tname[]={"Alpha", "Beta", "Gamma",  "Delta", "Epsilon", 
               "Zeta",  "Eta","Theta"};
int init()
{
   int i,j; 
   PROC *p;

   printf("Griffin's MTX initializing...."); 
   for (i=0; i<NPROC; i++)
   {
       p = &proc[i];
       p->pid = i;
       p->status = FREE;
       p->next = &proc[i+1];
       strcpy(p->name, tname[i]);
       printf("%d %s ", p->pid, proc[i % 4].name);
   }
   printf("\n");

   proc[NPROC-1].next = 0;
   freeList = &proc[0];   
   readyQueue = 0; // readyQueue initially NULL 

   // create and run P0
   p = getproc();
   p->pri = 0;
   p->status = READY;
   p->inkmode = 1;
   running = p;
}
   
int body()
{
  char c, CR, buf[64];
  while(1)
  {
    printf("=======================================\n");
    printQueue(readyQueue);      

    printf("P%d (%s) in Kmode\n", running->pid, running->name);
    printf("input a command (s|f|u|q|i|o) : ");
    c=getc(); putc(c); CR=getc(); putc(CR);
    switch(c)
    {
      case 's' : 
        tswitch(); 
        printf("\nReturned from tswitch\n"); 
        break;
      case 'u' : 
        printf("\nP%d ready to go U mode\n", running->pid);
        goUmode(); 
        break;
      case 'f':  
        do_kfork();  
        break;
      case 'q' : 
        printf("Attempting to call kexit()\n"); 
        kexit();   
        printf("Flag #2\n"); 
        break;
      case 'i' : 
        iline();   
        break;
      case 'o' : 
        oline();   
        break;
      }
   }
}

/***************************************************************
  myfork() creates a child task and returns the child pid.
  When scheduled to run, the child task resumes to body(pid) in 
  K mode. Its U mode environment is set to segment.
****************************************************************/
int myfork()
{  
   int child;
   child = kfork("/bin/u1");
   if (child < 0)
   {
       printf("myfork failed\n"); 
       return;
   } 
   printf("task %d return from myfork() : child=%d\n", running->pid, child);
}

int int80h(); 
int s0inth();  // int s1inth() // for serial port 1

int set_vec(vector, addr) ushort vector, addr;
{
    ushort location,cs;
    location = vector << 2;
    cs = getcs();
    put_word(addr, 0, location);
    put_word(cs,0,location+2);
}

main()
{
    int pid;
    resetVideo();
    printf("\nWelcome to Griffin's 460 Multitasking System\n");
    printf("initializing......");
    init();
    printf("complete\n");

    set_vec(80, int80h);    // Install interrupt handler 

    printf("P0 forks P1\n"); 

    pid = myfork("/bin/u1"); 



    set_vec(12, s0inth);  // Install handler for serial port 0 
    //set_vec(11, s1inth); // for second serial port at 0x2F8

    sinit();    // Initialize serial ports 

    while(1)
    {
       if (readyQueue)
          tswitch(); 
    }
    printf("all dead, happy ending\n");
}

int scheduler()
{ 
  if (running->status == READY)
  {
    enqueue(&readyQueue,running);
  }

  running = dequeue(&readyQueue);
}


/* Output a line to the serial port */ 
int oline()
{
  // assume port = 0;
  sputline(0, "serial line from Griffin in Kmode\n\r");
}

// kline is the circular buffer for serial input and output
char kline[64];
/* Input a line from the serial port */ 
int iline()
{
  sgetline(0, kline);
  printf("\nkline=%s\n", kline);
}
