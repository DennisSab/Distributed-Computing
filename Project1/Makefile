CC = gcc
CFLAGS = -pthread -Wall -Wextra -g
TARGET = project
SRCS = main.c init.c makeRes.c print.c controlPhase1.c phase2.c controlPhase2.c barrier.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

main.o: main.c header.h
	$(CC) $(CFLAGS) -c $<

init.o: init.c header.h
	$(CC) $(CFLAGS) -c $<

makeRes.o: makeRes.c header.h
	$(CC) $(CFLAGS) -c $<

print.o: print.c header.h
	$(CC) $(CFLAGS) -c $<

controlPhase1.o: controlPhase1.c header.h
	$(CC) $(CFLAGS) -c $<

phase2.o: phase2.c header.h
	$(CC) $(CFLAGS) -c $<

controlPhase2.o: controlPhase2.c header.h
	$(CC) $(CFLAGS) -c $<

barrier.o: barrier.c header.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
