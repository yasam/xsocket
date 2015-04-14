OBJS=xsocket.o
TARGETS=echod client

CC=$(CROSS_COMPILE)gcc
GCC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld

CFLAGS=-Wall

all:$(OBJS) $(TARGETS)


echod:$(OBJS) echod_main.o
	$(GCC) -o $@ $^ 

client:$(OBJS) client_main.o
	$(GCC) -o $@ $^ 

clean:
	rm -f *.o $(OBJS) $(TARGETS)


