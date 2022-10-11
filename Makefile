include config.mk

REQ = controller

COM = authentication\
      UI_API\
      p2

default: $(BIN)

.c.o:
	g++ $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BIN): $(REQ:=.o) $(COM:=.o)
	g++ $(LDFLAGS) $(LDLIBS) $(REQ:=.o) $(COM:=.o) -o $@

install: $(BIN)
	mkdir -p $(DEST)/bin
	cp -f cstatusbar $(DEST)/bin
	chmod 755 $(DEST)/bin/$(BIN)

uninstall:
	rm -f $(DEST)/bin/$(BIN)

clean:
	rm -f $(BIN) modules/*.o *.o

.PHONY: install uninstall clean 
