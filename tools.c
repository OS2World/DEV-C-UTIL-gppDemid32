#define INCL_DOSERRORS
#define INCL_DOSMEMMGR
#include <os2.h>

#include "tools.h"

PVOID* heapBase=NULL;

#if !defined (__HEAPSIZE__)
#define HEAPSIZE  64*1024
#endif

unsigned long SysCreateHeap(unsigned long size)
{
   APIRET rc;
do {
   rc=DosAllocMem((PVOID)&heapBase,size,PAG_READ | PAG_WRITE | OBJ_TILE);
   if (rc) {
      break;
   } /* endif */
   rc=DosSubSetMem( heapBase,
                    DOSSUB_INIT | DOSSUB_SPARSE_OBJ | DOSSUB_SERIALIZE,
                    size);
   if (rc) {
      DosFreeMem(heapBase);
      heapBase=0;
   } /* endif */
} while ( 0 ); /* enddo */
return rc;
}

unsigned long SysDestroyHeap( void )
{
   APIRET rc=0;
if (heapBase!=NULL) {
   rc =DosSubUnsetMem(heapBase);
   rc|=DosFreeMem(heapBase);
} /* endif */
return rc;
}


void* SysAlloc(unsigned long size)
{
   PULONG h=NULL;
   APIRET rc;
if (heapBase==NULL) {
   SysCreateHeap(HEAPSIZE);
}
rc=DosSubAllocMem(heapBase,(PVOID)&h,size+4);
if (rc==0) {
   *h=size+4;
} /* endif */
return (rc==0 ? (PVOID)(h+1) : NULL);
}

unsigned long SysFree(void* p)
{
   PULONG h=p;
   APIRET rc=ERROR_INVALID_PARAMETER;
if (h) {
   --h;
   rc=DosSubFreeMem(heapBase,(PULONG)h,*h);
}
return rc;
}


