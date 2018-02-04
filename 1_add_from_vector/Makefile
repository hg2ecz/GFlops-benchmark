CC=gcc
CFLAGS=-Wall -Ofast -march=native -fopenmp
LDFLAGS=-s -lgomp

OBJS=gflops-test.o gflops.o
TARGET=gflops

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
