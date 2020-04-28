CC=gcc
CFLAGS=-Wall -Ofast -funroll-all-loops
CFLAGS+=-march=native
LDFLAGS=-s -lm

OBJS=test.o benchmark.o
OBJS2=test2.o benchmark.o
TARGET=benchmark
TARGET2=benchmark-memcpy

all: $(OBJS) $(OBJS2)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)
	$(CC) $(OBJS2) $(LDFLAGS) -o $(TARGET2)

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(OBJS2) $(TARGET2)

test:
	./$(TARGET)
	./$(TARGET2)