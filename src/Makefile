CC = gcc
CFLAGS = -g -ggdb
JSONC = /usr/local/include/json-c

.SUFFIXES: .c .o
all:run

run:gateway_main.o communicator.o otp.o lcd.o write_conf.o init.o state_mgr
	$(CC) -o $@ gateway_main.o communicator.o otp.o lcd.o write_conf.o init.o -lpthread -ljson-c -lwiringPi -lwiringPiDev

communicator.o:
	$(CC) -c communicator.c -I$(JSONC)

otp.o:
	$(CC) -c otp.c -lpthread -lwiringPi -lwiringPiDev -std=c99

lcd.o:
	$(CC) -c lcd.c -lwiringPi -lwiringPiDev


write_conf.o:
	$(CC) -c write_conf.c	

init.o:
	$(CC) -c init.c -lwiringPi -lwiringPiDev

state_mgr:
	$(CC) $(CFLAGS) -o state_mgr state_mgr.c -ljson-c -I$(JSONC)
clean:
	 rm -f *.o

communicator.o : communicator.c communicator.h gateway_main.h state_mgr.h
gateway_main.o : gateway_main.c gateway_main.h communicator.h write_conf.h lcd.h otp.h
state_mgr : state_mgr.c state_mgr.h
