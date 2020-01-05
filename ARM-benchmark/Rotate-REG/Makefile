CC=gcc
CFLAGS=-Wall -Ofast -march=native -funroll-all-loops
LDFLAGS=-s -lpthread

CFLAGS+=-DTHREADNUM=4
CFLAGS+=-DITERNUM_MILL=133

OBJS=rotate.o main.o
TARGET=rotate_reg

all: $(OBJS)
	$(CC) $(CFLAGS) rotate.c -S    # show asm code
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJS) rotate.s $(TARGET)
