include config.mk

# Dynamic library
DLIB   = $(LIB:=.so)
# Static library
SLIB   = $(LIB:=.a)

#########
# RULES #
#########

default: $(BIN)

lib : slib dlib

slib: $(SLIB)

dlib: $(DLIB)

move: smove dmove

################################
# COVERAGE CODE SUPPORT (gcov) #
################################

cov: ccov ldcov

ccov: $(OBJ:=.c) $(MAIN:=.c)
	$(CC) $(CFLAGS) $(COVFLAGS) -c $^

ldcov: $(OBJ:=.o)
	$(CC) $(LDFLAGS) $(COVLDFLAGS) $(OBJ:=.o) $(MAIN:=.o) -o $(BIN)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ:=.o) $(MAIN:=.o) $(HEADER:=.h)
	$(CC) $^ -o $@ $(LDFLAGS)

# Generate dynamic library rule
$(DLIB): $(OBJ:=.o)
	$(CC) -shared $^ -o $@

# Generate static library rule
$(SLIB): $(OBJ:=.o)
	$(AR) -rcs $@ $^

smove:
	mkdir -p $(LIBDEST)
	mkdir -p $(INCDEST)
	cp -f $(SLIB) $(LIBDEST)
	cp -f $(HEADER:=.h) $(INCDEST)
dmove:
	mkdir -p $(LIBDEST)
	mkdir -p $(INCDEST)
	cp -f $(DLIB) $(LIBDEST)
	cp -f $(HEADER:=.h) $(INCDEST)

run: default
	@./$(BIN)

install: $(BIN)
	mkdir -p $(DEST)/bin
	cp -f cstatusbar $(DEST)/bin
	chmod 755 $(DEST)/bin/$(BIN)

uninstall:
	rm -f $(DEST)/bin/$(BIN)

clean:
	rm -f $(BIN) modules/*.o *.o *.so *.a

cleanall:
	rm -f $(BIN) modules/*.o *.o *.so *.a *.g* shadow

.PHONY: install uninstall clean run move lib slib dlib
