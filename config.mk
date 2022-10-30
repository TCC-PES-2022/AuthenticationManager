BIN        = login
CC         = cc
AR         = ar
CFLAGS     = -Wall -Wextra -fPIC
COVFLAGS   = -fprofile-arcs -ftest-coverage -g
LDFLAGS    = -lgcrypt -lgpg-error
COVLDFLAGS = -lgcov --coverage
DESTDIR    ?= /tmp
LIBDEST    ?= $(DESTDIR)/lib
INCDEST    ?= $(DESTDIR)/include

HEADER = iauthentication

MAIN   = main

OBJ    = authentication

LIB    = libauthentication
