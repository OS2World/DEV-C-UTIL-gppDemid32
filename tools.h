#if !defined (__TOOLS_H__)
#define __TOOLS_H__ 1

#define HEAPSIZE  64*1024

unsigned long SysCreateHeap(unsigned long size);
unsigned long SysDestroyHeap( void );
void*         SysAlloc(unsigned long size);
unsigned long SysFree(void* p);

#endif
