#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include "mem.h"

int main()
{
  int exit_loop=1;
  int memInit_return;
  int *ptr;
  while(exit_loop==1)
  {
    int ch;
    printf("1. Init mem 2.Mem_Alloc 3.free 0.Exit\n");
    scanf("%d",&ch);
    if(ch==1)
    {
      memInit_return=Mem_Init(100,1);
      if(memInit_return==-1)
      break;
    }
    else if(ch==2)
    {
      printf("Enter size for Mem_Alloc\n");
      int i=0;
      scanf("%d",&i);
      ptr=Mem_Alloc(sizeof(int)*i);
      if(ptr==NULL)
      {
        printf("Rcvd NULL ptr\n");
        break;
      }
      printf("Rcvd ptr : %p\n",ptr );

      ptr[0]=10;
      ptr[6]=121;

      printf("%d\n",ptr[6]);

    }
    else if(ch==3)
    {
      printf("Inside free\n");
      Mem_Free(ptr);
    }
    else if(ch==0)
    {
      break;
    }

    printf("Enter 0 to exit | 1 to continue \n");
    scanf("%d",&exit_loop);
  }

printf("Program exiting\n");
}
