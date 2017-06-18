all: server sender viewer

server: mensagem.o server.c
	gcc -o server server.c mensagem.o

sender: mensagem.o clientConnection.o sender.c
	gcc -o sender sender.c mensagem.o clientConnection.o

viewer: mensagem.o clientConnection.o viewer.c
	gcc -o viewer viewer.c mensagem.o clientConnection.o

mensagem.o: mensagem.h mensagem.c
	gcc -o mensagem.o -c mensagem.c

clientConnection.o: clientConnection.h clientConnection.c
	gcc -o clientConnection.o -c clientConnection.c

clean:
	rm -rf *.o
