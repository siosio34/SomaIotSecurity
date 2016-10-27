CC = gcc
JSONC = /usr/local/include/json-c

.SUFFIXES: .c .o
all:run

run:gateway_main.o communicator.o otp.o lcd.o write_conf.o
	$(CC) -o $@ gateway_main.o communicator.o otp.o lcd.o write_conf.o -lpthread -ljson-c -lwiringPi -lwiringPiDev

communicator.o:
	$(CC) -c communicator.c -I$(JSONC)

otp.o:
	$(CC) -c otp.c -lpthread -lwiringPi -lwiringPiDev -std=c99

lcd.o:
	$(CC) -c lcd.c -lwiringPi -lwiringPiDev

write_conf.o:
	$(CC) -c write_conf.c	

clean:
	 rm -f *.o

communicator.o : communicator.c communicator.h gateway_main.h
gateway_main.o : gateway_main.c gateway_main.h communicator.h write_conf.h lcd.h otp.h
