ARCH := $(shell getconf LONG_BIT)

CMD_32 := gcc main_32bit.o processQueue_32bit.o -pthread CPUs.o -o PEX3
CMD_64 := gcc main.o processQueue.o -pthread CPUs.o -o PEX3

CMD := $(CMD_$(ARCH))

PEX3: main.o processQueue.o CPUs.o makefile
	$(CMD)

CPUs.o: CPUs.c CPUs.h processQueue.h makefile
	gcc -c CPUs.c

clean:
	-rm CPUs.o
	-rm *~
	-rm PEX3
	clear

