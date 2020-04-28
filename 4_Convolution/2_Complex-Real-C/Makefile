CC=gcc
CFLAGS=-Wall -Ofast -funroll-all-loops
CFLAGS+=-march=native
LDFLAGS=-s -lm

OBJS=test.o benchmark.o
TARGET=benchmark

all: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

test:
	./$(TARGET)
