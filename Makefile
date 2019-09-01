CC = gcc
CFLAGS = -Wall
DEPPS = info.h
DEL = main server.o threadworker.o opserver.o hashtable.o mainclient client.o libclient.o

.PHONY: all

all: clean main mainclient

main: server.o threadworker.o opserver.o hashtable.o
	$(CC) -pthread -o main server.o threadworker.o opserver.o hashtable.o

server.o: server.c $(DEPPS)
	$(CC) $(CFLAGS) -c -g server.c

threadworker.o: threadworker.c $(DEPPS)
	$(CC) $(CFLAGS) -c -g threadworker.c

opserver.o: opserver.c $(DEPPS)
	$(CC) $(CFLAGS) -c -g opserver.c

hashtable.o: hashtable.c $(DEPPS)
	$(CC) $(CFLAGS) -c -g hashtable.c

mainclient: client.o libclient.o
	$(CC) -o mainclient client.o libclient.o

client.o: client.c $(DEPPS)
	$(CC) $(CFLAGS) -c -g client.c

libclient.o: libclient.c $(DEPPS)
	$(CC) $(CFLAGS) -c -g libclient.c

perm:
	@chmod +x test.sh

clean:
	@rm -f $(DEL) objstore.sock testout.log formtestout.log *~
	@rm -rf data

test:
	make all
	./main &
	./test.sh
	killall -SIGTERM main 

