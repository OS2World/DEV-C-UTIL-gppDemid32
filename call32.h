#if !defined (__CALL32_H__)
#define __CALL32_H__ 1


static __inline__ unsigned FlatToSel(unsigned p)
{
   register unsigned rc __asm__("%eax");
__asm__ __volatile__( "movl %0,%%eax \n\t"
                      "call DosFlatToSel \n\t"
                      :// "=g"(rc)
                      :"g"(p) : "%eax", "%ebx", "%ecx");
return rc;
}

static __inline__ unsigned SelToFlat(unsigned p)
{
   register unsigned rc __asm__("%eax");
__asm__ __volatile__( "movl %0,%%eax \n\t"
                      "call DosSelToFlat \n\t"
                      :// "=g"(rc)
                      :"g"(p) : "%ebx", "%ecx");
return rc;
}



#endif
