#include "mem.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

typedef struct mem_node {
  struct mem_node *next;
  struct mem_node *prev;
  int	  size;
  int available;
}node_t;

node_t* create_new_node(node_t** currNode,node_t** newNode,int size);
int search(node_t** freeptr);
void join(node_t **ptr);



node_t *head=NULL;
int freesize=0;

int Mem_Init(int sizeOfRegion,int debug)
{
  if(head!=NULL || sizeOfRegion<=0)
  {
    printf("Head allocated / sizeOfRegion <=0\n");
    return -1;
  }
  if(sizeOfRegion%getpagesize()!=0)
  {
    sizeOfRegion=sizeOfRegion+getpagesize() - (sizeOfRegion % getpagesize() );
    printf("New size is %d\n",sizeOfRegion);
  }
  int fd = open("/dev/zero", O_RDWR);
  void* ptr = mmap(NULL,sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (ptr == MAP_FAILED) { printf("Mapping failed\n"); return -1; }
  close(fd);
  head=(node_t*)ptr;
  head->next=NULL;
  head->prev=NULL;
  freesize=sizeOfRegion;
  printf("Head is at -- sizeOfRegion %p / %d\n",head,sizeOfRegion);
}

void *Mem_Alloc(int size)
{
  if(size%8!=0)
  {
    size=size+8 - (size%8);
    printf("New rqstd size is : %d / %d\n",size,freesize );
  }

  if(size>freesize)
  {
    printf("Size > freesize\n");
  return NULL;
}

  node_t *newNode=NULL;
  node_t *currNode=head;

  if(currNode->next==NULL)
  {
    newNode=create_new_node(&currNode,&newNode,size);
    printf("newNode  size  %p / %d\n",newNode,newNode->size );
    // return (void *)(newNode+32);
  }
  else
  { printf("Inside else\n");
    while(currNode->next!=NULL)
    { currNode=currNode->next;
       printf("Inside while\n");
    printf("currNode size / size+32  %d /  %d \n",currNode->size,size+32);
      if(currNode->size>size+32 && currNode->available==1)
      { printf("Inside if 1\n");
        newNode=create_new_node(&currNode,&newNode,size);
        break;
      }
      else if(currNode->size==size+32 && currNode->available==1)
      {
        printf("Inside if 2\n");
        newNode=create_new_node(&currNode,&newNode,size);
        break;
      }
      // currNode=currNode->next;
    }//while
    if(newNode==NULL)
    {
      newNode=create_new_node(&currNode,&newNode,size);
    }

  }//else
printf("freesize %d\n",freesize );
return (void *)(newNode+32);
} //Mem_Alloc

node_t *create_new_node(node_t** currNode,node_t** newNode,int size)
{
  node_t *curr=*currNode;
  node_t *new=*newNode;
  printf("Curr in splt %p\n",curr );
  new=(node_t*)(curr+sizeof(node_t));
  printf("new in splt %p\n",new );
  new->next=NULL;
  new->prev=curr;
  new->available=0;
  printf("new's prv %p\n",new->prev );
  new->size=size;
  curr->next=new;
  freesize=freesize-(sizeof(node_t)+size);
  return new;
}

int Mem_Free(void *ptr)
{
  printf("to free %p\n",ptr );
  node_t *ptr_to_search=(node_t*)ptr;
  int search_ptr=search(&ptr_to_search);
  if(!search_ptr)
  {
    printf("No ptr found\n");
    return -1;
  }
  else
  {
    printf("Found / %p\n",ptr_to_search );

    return 1;
  }


}

void join(node_t **ptr)
{
  // node_t *prevNode=NULL;
  // node_t *nextNode=NULL;
  // prevNode=(*ptr)->prev;
  // nextNode=(*ptr)->next;
  printf("To be freed ptr is %p\n",*ptr );
  (*ptr)->available=0;
  return;


}

int search(node_t** freeptr)
{
  node_t *curr=head;
  while(curr)
  {
    if(curr=*freeptr)
    {
      printf("Found\n");
    join(&(*freeptr));
    return 1;

    }
    curr=curr->next;
  }
  return 0;
}
