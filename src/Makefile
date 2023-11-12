Tgt := main.out
BldObjPath := ../build/obj
CFLAGS += -Icom -I../lib/log

vpath %.c *.c */*.c
vpath %.h */*.h
vpath %.a ../build/lib/*.a

Cs := $(wildcard *.c com/*.c)
Hs := $(wildcard com/*.h ../lib/**/*.h)
Obj := $(BldObjPath)/main.o $(BldObjPath)/com.o

$(Tgt): $(Obj)
	$(CC) -o $(BldObjPath)/$@ $^

$(Obj): $(Cs) $(Hs)
	-mkdir -p $(BldObjPath)
	$(CC) -o $(BldObjPath)/main.o -c main.c $(CFLAGS)
	$(CC) -o $(BldObjPath)/com.o -c com/com.c $(CFLAGS)

.PHONY: clean

clean:
	$(RM) $(BldObjPath)/*.out $(BldObjPath)/*.o
