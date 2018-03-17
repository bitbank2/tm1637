CFLAGS=-c -Wall -O2

all: libtm1637.a

libtm1637.a: tm1637.o
	ar -rc libtm1637.a tm1637.o ;\
	sudo cp libtm1637.a /usr/local/lib ;\
	sudo cp tm1637.h /usr/local/include

tm1637.o: tm1637.c tm1637.h
	$(CC) $(CFLAGS) tm1637.c

clean:
	rm *.o libtm1637.a

