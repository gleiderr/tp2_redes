all: server sender viewer

server: obj/mensagem.o server.c
	gcc -o server server.c obj/mensagem.o

sender: obj/mensagem.o obj/clientConnection.o sender.c
	gcc -o sender sender.c obj/mensagem.o obj/clientConnection.o

viewer: obj/mensagem.o obj/clientConnection.o viewer.c
	gcc -o viewer viewer.c obj/mensagem.o obj/clientConnection.o

obj/mensagem.o: mensagem.h mensagem.c
	gcc -o obj/mensagem.o -c mensagem.c

obj/clientConnection.o: clientConnection.h clientConnection.c
	gcc -o obj/clientConnection.o -c clientConnection.c

clean:
	rm -rf *.o
