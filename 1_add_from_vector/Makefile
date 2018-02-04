CC=gcc
CFLAGS=-Wall -Ofast -march=native -fopenmp
LDFLAGS=-s -lgomp

OBJS=gflops-test.o gflops.o
TARGET=gflops

all: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
