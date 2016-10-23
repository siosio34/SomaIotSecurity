CC = gcc
JSONC = /usr/local/include/json-c

.SUFFIXES: .c .o
all:run

run:gateway_main.o communicator.o otp.o
	$(CC) -o $@ gateway_main.o communicator.o otp.o -lpthread -ljson-c

communicator.o:
	$(CC) -c communicator.c -I$(JSONC)

clean:
	 rm -f communicator.o 

communicator.o : communicator.c communicator.h gateway_main.h
gateway_main.o : gateway_main.c gateway_main.h communicator.h write_conf.h lcd.h otp.h 
