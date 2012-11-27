#define debug 1
int body(); 
int kfork(); 
//int goUmode(); 

/************************************************************
	 				Fork functions begin
*************************************************************/ 
int do_kfork()
{
   int new;
   new = kfork();
   if (new < 0)
       printf("kfork failed\n");
   else
       printf("P%d return from kfork() : child = %d\n",
               running->pid, new);
}

/***********************************************************
  kfork() creates a child proc and returns the child pid.
  When scheduled to run, the child process resumes to body();
************************************************************/
int kfork()
{
  u16 segment; 
  PROC *p; 
  int j; 
  int index; 
  printf("\nGriffin's kfork():\n"); 
  printf("Forking child off P%d\n", running->pid);

  p = getproc();  // get free PROC from freeList

  if(p == 0) // No free PROCs found
  {
    printf("ERROR: fork failed to find available proc\n"); 
    return -1; 
  }

  // Initialize the new proc
  p->status = READY;
  p->ppid = running->pid; 

  
  // Zero out all of the PROC's stack registers 
  for (j=1; j<10; j++)      
  {
    p->kstack[SSIZE - j] = 0;          
    
  }
  p->kstack[SSIZE-1] = (int)body;          // Fill in return address
  p->ksp = &(p->kstack[SSIZE-9]);         // ksp -> top of kstack

  
  if(debug)
    printf("Calling enqueue from Griffin's kfork() to insert P%d into readyQueue\n", p->pid); 

  enqueue(&readyQueue, p);  // insert p into readyQueue by priority

  /* Added code for lab 4 */ 
  segment = (p->pid + 1) * 0x1000;  /* Compute PROCs segment */ 
  printf("\n***************** Loading user mode image *****************\n"); 
  load("u1", segment);  /* Every newly forked PROC has u1 as it's Umode image but in it's own segment */ 
  printf("\n"); 

  /* ---------------------------------------------------------------------------------------------------------------
  ustack is at the high end of the segment and should mirror the diagram below: 
  (copied, pasted & slightly modified from his notes)

      (LOW)  uSP                                | by INT 80  |     HIGH
     -------------------------------------------------------------------------
           |uDS|uES| di| si| bp| dx| cx| bx| ax|uPC|uCS|flag |XXXXXXXXXXXXXXXXX
     --------------------------------------------------------------------------
         |seg|seg| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |seg|0x0200|XXXXXXXXXXXXXXXXX
            -12 -11 -10  -9  -8  -7  -6  -5  -4  -3  -2  -1  | 0 

  ----------------------------------------------------------------------------------------------------------------*/ 
  
  /* Populate uPC, ax, bx, cx, dx, bp, si, di registers */ 
  for(j=1; j<=12; j++)
    put_word(0x0, segment, -2*j);   // Each register holds 2 bytes

  put_word(0x0200, segment, -2*1);  /* flag */ 
  put_word(segment, segment, -2*2);   /* uCS */ 
  put_word(segment, segment, -2*11);  /* uES */ 
  put_word(segment, segment, -2*12);  /* uDS */ 

  /* set proc.uss and proc.usp */ 
  p->usp = -2*12;     /* top of stack */ 
  p->uss = segment;     /* user stack segment */ 

  if(debug)
    printf("kfork forked P%d off of parent P%d\n", p->pid, p->ppid); 

  printf("Proc %d forked a child %d at segment %x\n",
          running->pid, p->pid, segment);

  return (p->pid); 
}

/*******************************
  forkP1()
********************************/ 
int forkP1()
{
  u16 segment; 
  PROC *p; 
  int j; 
  int index; 
  printf("Forking child off P%d\n", running->pid);

  p = getproc();  // get free PROC from freeList

  if(p == 0) // No free PROCs found
  {
    printf("ERROR: fork failed to find available proc\n"); 
    return -1; 
  }
    

  // Initialize the new proc
  p->status = READY;
  p->ppid = running->pid; 

  
  // Zero out all of the PROC's stack registers 
  for (j=1; j<10; j++)      
  {
    p->kstack[SSIZE - j] = 0;          
    
  }
  p->kstack[SSIZE-1] = (int)body;          // Fill in return address
  p->ksp = &(p->kstack[SSIZE-9]);         // ksp -> top of kstack

  
  if(debug)
    printf("Calling enqueue from kfork() to insert P%d into readyQueue\n", p->pid); 

  enqueue(&readyQueue, p);  // insert p into readyQueue by priority


  // Create user mode image for P1 
  segment = (p->pid + 1) * 0x1000;  /* Compute PROCs segment */ 
  printf("\n***************** Loading user mode image *****************\n"); 
  load("u1", segment);  /* Every newly forked PROC has u1 as it's Umode image but in it's own segment */ 
  printf("\n"); 

  /* ---------------------------------------------------------------------------------------------------------------
  ustack is at the high end of the segment and should mirror the diagram below: 
  (copied, pasted & slightly modified from his notes)

      (LOW)  uSP                                | by INT 80  |     HIGH
     -------------------------------------------------------------------------
           |uDS|uES| di| si| bp| dx| cx| bx| ax|uPC|uCS|flag |XXXXXXXXXXXXXXXXX
     --------------------------------------------------------------------------
           |seg|seg| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |seg|0x0200|XXXXXXXXXXXXXXXXX
            -12 -11 -10  -9  -8  -7  -6  -5  -4  -3  -2  -1  | 0 

  ----------------------------------------------------------------------------------------------------------------*/ 
  
  /* Populate uPC, ax, bx, cx, dx, bp, si, di registers */ 
  // Clear all registers
  for(j=1; j<=12; j++)
    put_word(0x0, segment, -2*j);   // Each register holds 2 bytes

  put_word(0x0200, segment, -2*1);  /* flag */ 
  put_word(segment, segment, -2*2);   /* uCS */ 
  put_word(segment, segment, -2*11);  /* uES */ 
  put_word(segment, segment, -2*12);  /* uDS */ 

  /* set proc.uss and proc.usp */ 
  p->usp = -2*12;     /* top of stack */ 
  p->uss = segment;     /* user stack segment */ 

  if(debug)
    printf("kfork forked P%d off of parent P%d\n", p->pid, p->ppid); 

  printf("Proc %d forked a child %d at segment %x\n",
          running->pid, p->pid, segment);

  return (p->pid);
}
/************************************************************
	 				Fork functions end
*************************************************************/ 

int kexec()
{

}

