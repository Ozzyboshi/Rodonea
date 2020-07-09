#
# makefile for huffman
#
# Bruno Costa - 17 Apr 91 - 17 Apr 91
#

EXE = huffman
OBJS = huffman.o encoders.o bits.o

.c.o:
	lc -cusf -O $*

#$(EXE): $(OBJS)
#	echo >T:$(EXE).lnk "LIB:catch.o $(OBJS)"
#	echo >>T:$(EXE).lnk "to $(EXE) lib LIB:lc.lib batch sc sd addsym"
#	blink with T:$(EXE).lnk

$(EXE): $(OBJS)
	echo >T:$(EXE).lnk "LIB:c.o $(OBJS)"
	echo >>T:$(EXE).lnk "to $(EXE) lib LIB:lc.lib batch sc sd nd"
	blink with T:$(EXE).lnk
