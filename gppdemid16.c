#include <call32.h>

extern unsigned long InitDemangleID32
   (const char* init);

extern unsigned long DemangleID32
   (const char *src, char *dst, unsigned long dst_size);

unsigned Thunk_InitDemangleID(unsigned char* args)
{
   const char* init= (const char*) SelToFlat(((unsigned long*)args)[0]);
return InitDemangleID32(init);
}

unsigned Thunk_DemangleID(unsigned char* args)
{
   const char* src;
   char* dst;
   unsigned long dst_size;
src= (const char*) SelToFlat(((unsigned long*)args)[2]);
dst= (char*)       SelToFlat(((unsigned long*)args)[1]);
dst_size=                    ((unsigned long*)args)[0];
return DemangleID32 (src,dst,dst_size);
}

