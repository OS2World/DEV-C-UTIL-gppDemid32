CC= gcc -Zomf -s
LD= $(CC)
CFLAGS= -O2 -m486
LFLAGS= -Zdll -Zmap -Zno-rte  -Zlinker "/MAP:FULL" 

.c.obj:
      $(CC) $(CFLAGS) -c $<
.asm.obj:
      alp -Fl $<


OBJ= cplus-de.obj gppdemid32.obj gppdemid16.obj glue1632.obj tools.obj \
     call1632.obj

all: gppdemid.dll

gppdemid.dll: $(OBJ) gppdemid32.def
       $(LD) $(LFLAGS) -o $@ gppdemid32.def $(OBJ)


clean:
	-del *.obj
	-del *.dll
	-del *.exe
	-del *.lst
	-del *.map
