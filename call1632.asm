         .386
         .387
   .MODEL FLAT

TEXT32   SEGMENT USE32 PUBLIC DWORD 'CODE'
TEXT32   ENDS

DATA32   SEGMENT USE32 PUBLIC DWORD 'DATA'
         ORG 0
flatPtr  LABEL NEAR
DATA32   ENDS

DGROUP   GROUP DATA32


TEXT32   SEGMENT
         ASSUME CS:FLAT,DS:NOTHING,ES:NOTHING
         PUBLIC _Call32
_Call32  PROC NEAR
   ; Stack ist 16-Bit beim Aufruf
   ; EBX Grî·e der Parameter in Bytes
   ; EDX Addresse der aufzurufenden Funktion
   ; Stack:
   ;        Parameter
   ;        Sel         --> RetAddr
   ;        Off         --> RetAddr
   ; 1. 32-Bit Stack bauen
   mov ecx,ss
   and ecx,0FFFFH
   shl ecx,16-3   ; Selector-Bits ausblenden und gleich verschieben
   mov cx,sp      ; ECX = Flat --> SS:SP
   ;lea eax,FLAT:DATA32   ; nur fÅr ALP wegen Bug
   ; mov eax, SEG FLAT:flatPtr ; fÅr MASM und TASM
   ASSUME DS:FLAT,CS:NOTHING
   lea eax,FLAT:DATA32
   ASSUME CS:FLAT,DS:NOTHING
   mov ss,eax
   mov esp,ecx
   ; Jetzt ist Stack 32-Bit
   test esp,3
   jz stDWORD
   and esp, 0FFFFFFFCH ; Stack auf 4 Byte ausrichten
stDWORD:
   push ds
   push es
   mov ds,eax     ; FLAT-Sel in ds laden
   mov es,eax     ; FLAT-Sel in es laden
   push ecx       ; alten Stackpointer sichern

   push ebp
   mov ebp,esp

   add ecx,4      ; Pointer auf Parameter erzeugen
   push ecx
   call edx    ; eax=(edx)((void*)args);

   mov esp,ebp
   pop ebp

   pop ecx        ; alten Stackpointer zurÅckholen
   mov edx,[ecx]  ; in EDX RÅckkehraddresse
   mov [ecx+ebx],edx ; RÅckkehraddresse an die richtige Stelle schreiben

   pop es         ; es wiederherstellen
   pop ds         ; ds wiederherstellen

   mov esp,ecx    ; ESP wieder mit OriginalWert laden
   add esp,ebx    ; Stack:       SEL:OFF RetAddr d.h. Parameter entfernen
   mov edx,esp
   shr edx,16-3
   or edx, 7      ; TILED Selector erzeugen
   ; Stack 16-Bit
   mov ss,edx    ;
   and esp,0FFFFH ;   High Bits ausblenden
   shld edx,eax,16 ; Ergebnis erzeugen
   db 66H        ;
   retf
_Call32  ENDP

TEXT32   ENDS

END
