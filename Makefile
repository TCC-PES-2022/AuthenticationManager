BIN      = log
CC       = cc
AR       = ar
CFLAGS   = -Wall -Wextra -O3 -march=native -fPIC
LFLAGS   = -shared
LDFLAGS  = -lgcrypt -lgpg-error
LIBDEST  = ${HOME}/pes/lib
INCDEST  = ${HOME}/pes/include

HEADER = iauthentication

OBJ    = authentication\
         main

SOBJ   = authentication

LIB    = $(SOBJ:=.so)

default: $(BIN)

lib: $(LIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ:=.o) $(HEADER:=.h)
	$(CC) $(OBJ:=.o) $(LDFLAGS) -o $@

$(LIB): $(OBJ:=.o)
	$(CC) $(LFLAGS) $(LDFLAGS) $^ -o $@
	$(AR) -rc $(SOBJ:=.a) $^

move:
	cp -f $(SOBJ:=.so) $(SOBJ:=.a) $(LIBDEST)
	cp -f $(HEADER:=.h) $(INCDEST)

run: default
	@./log

install: $(BIN)
	mkdir -p $(DEST)/bin
	cp -f cstatusbar $(DEST)/bin
	chmod 755 $(DEST)/bin/$(BIN)

uninstall:
	rm -f $(DEST)/bin/$(BIN)

clean:
	rm -f $(BIN) modules/*.o *.o *.so *.a

.PHONY: install uninstall clean run move
