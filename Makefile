include config.mk

COM = main\
      authentication

default: $(BIN)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BIN): $(COM:=.o)
	$(CXX) $(LDFLAGS) $(LDLIBS) $(COM:=.o) -o $@

test: default
	@echo
	@./$(BIN) -d

install: $(BIN)
	mkdir -p $(DEST)/bin
	cp -f cstatusbar $(DEST)/bin
	chmod 755 $(DEST)/bin/$(BIN)

uninstall:
	rm -f $(DEST)/bin/$(BIN)

clean:
	rm -f $(BIN) modules/*.o *.o

.PHONY: install uninstall clean 
