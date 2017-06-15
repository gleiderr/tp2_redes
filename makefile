all: server sender viewer

server: cabecalho.h server.c
	gcc -o server server.c

sender: cabecalho.h sender.c
	gcc -o sender sender.c

viewer: cabecalho.h viewer.c
	gcc -o viewer viewer.c

clean:
	rm -rf *.o
