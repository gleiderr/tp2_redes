all: server sender viewer

server: mensagem.o server.c
	gcc -o server server.c mensagem.o

sender: mensagem.o sender.c
	gcc -o sender sender.c mensagem.o

viewer: mensagem.o viewer.c
	gcc -o viewer viewer.c mensagem.o

mensagem.o: mensagem.h mensagem.c
	gcc -o mensagem.o -c mensagem.c

clean:
	rm -rf *.o
