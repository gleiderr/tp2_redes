all: server sender viewer

server: obj/mensagem.o obj/serverConnection.o src/server.c
	gcc -o server src/server.c obj/mensagem.o obj/serverConnection.o

sender: obj/mensagem.o obj/clientConnection.o src/sender.c
	gcc -o sender src/sender.c obj/mensagem.o obj/clientConnection.o

viewer: obj/mensagem.o obj/clientConnection.o src/viewer.c
	gcc -o viewer src/viewer.c obj/mensagem.o obj/clientConnection.o

obj/mensagem.o: src/mensagem.h src/mensagem.c
	gcc -o obj/mensagem.o -c src/mensagem.c

obj/clientConnection.o: src/clientConnection.h src/clientConnection.c
	gcc -o obj/clientConnection.o -c src/clientConnection.c

obj/serverConnection.o: src/serverConnection.h src/serverConnection.c
	gcc -o obj/serverConnection.o -c src/serverConnection.c

clean:
	rm -rf *.o
