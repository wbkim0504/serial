.SUFFIXES : .c .o

OBJS = serial.o
SRCS = $(OBJS:.o=.c)
TARGET = a.out

CC = gcc
CFLAGS = -c
LDFLAGS =
LDLIBS = -lwiringPi

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDLIBS)
	rm -f $(OBJS)

.c.o: 
	$(CC) $(INC) $(CFLAGS) $<

clean:
	rm -f $(TARGET) $(OBJS)

