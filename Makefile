CC=gcc
CCFLAGS=-D_DEFAULT_SOURCE -D_XOPEN_SOURCE -D_BSD_SOURCE -std=c11 -pedantic -Wvla -Wall -Werror

all: server client 



server : server.o utils_v1.o jeu.o network.o 
	$(CC) $(CCFLAGS) -o server server.o utils_v1.o jeu.o network.o

server.o: server.c utils_v1.h structure.h network.h jeu.h
	$(CC) $(CCFLAGS) -c server.c 

client : client.o utils_v1.o network.o jeu.o
	$(CC) $(CCFLAGS) -o client client.o utils_v1.o network.o jeu.o

client.o: client.c utils_v1.h structure.h network.h jeu.h
	$(CC) $(CCFLAGS) -c client.c

network.o: network.c utils_v1.h structure.h 
	$(CC) $(CCFLAGS) -c network.c

jeu.o: jeu.c utils_v1.h structure.h
	$(CC) $(CCFLAGS) -c jeu.c

utils_v1.o: utils_v1.c utils_v1.h
	$(CC) $(CCFLAGS) -c utils_v1.c 

clear :
		clear

clean :
	rm -f *.o
	rm -f $(ALL)
