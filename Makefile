BIN      = log
CC       = cc
CFLAGS   = -Wall -Wextra -O3 -march=native -fPIC
LFLAGS   = -shared
LDFLAGS  = -lgcrypt -lgpg-error
LIBDEST  = ${HOME}/pes/lib
INCDEST  = ${HOME}/pes/include

HEADER = iauthentication

OBJ    = authentication\
         main

SOBJ   = authentication

SLIB   = $(SOBJ:=.so)

default: $(BIN)

shared: $(SLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ:=.o) $(HEADER:=.h)
	$(CC) $(OBJ:=.o) $(LDFLAGS) -o $@

$(SLIB): $(OBJ:=.o)
	$(CC) $(LFLAGS) $(LDFLAGS) $^ -o $@

move:
	cp -f $(SOBJ:=.so) $(LIBDEST)
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
	rm -f $(BIN) modules/*.o *.o *.so

.PHONY: install uninstall clean run move
