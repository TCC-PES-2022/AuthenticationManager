# version
VERSION = 0.1

BIN      = vsfls
CC       = gcc
CXX      = g++
# paths
DEST = /usr/local

X11INC = /usr/local/include
X11LIB = /usr/local/lib

CFLAGS   = -Wall -Wextra -Os -march=native
LDFLAGS  = -lgcrypt
