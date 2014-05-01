   .386p
   .387

TEXT32   SEGMENT USE32 DWORD PUBLIC 'CODE'
         ASSUME CS:FLAT,DS:FLAT,ES:FLAT,SS:NOTHING
         EXTERN Thunk_InitDemangleID:BYTE
         EXTERN Thunk_DemangleID:BYTE
         EXTERN _Call32:BYTE
TEXT32   ENDS

TEXT16   SEGMENT USE16 PARA PUBLIC 'CODE'
         ASSUME CS:TEXT16
         PUBLIC INITDEMANGLEID
         PUBLIC DEMANGLEID
INITDEMANGLEID PROC FAR
               mov ebx,4
               mov edx,offset FLAT:Thunk_InitDemangleID
               jmp far ptr FLAT:_Call32
               retf 4
INITDEMANGLEID ENDP

DEMANGLEID     PROC FAR
               mov ebx,3*4
               mov edx,offset FLAT:Thunk_DemangleID
               jmp far ptr FLAT:_Call32
               retf 3*4
DEMANGLEID     ENDP
TEXT16   ENDS

END

