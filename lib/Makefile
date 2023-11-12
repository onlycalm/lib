Tar := log.a
BldLibPath := ../build/lib/

vpath %.c log
vpath %.h log
vpath %.a log

Cs := $(wildcard */*.c)

$(Tar): $(Cs)
	-mkdir -p $(BldLibPath)
	$(AR) rcs $(BldLibPath)$@ $^

.PHONY: clean

clean:
	$(RM) $(BldLibPath)$(Tar)
