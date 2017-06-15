#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "cabecalho.h"

int openSocket(char const* addr) {
    int i, s;
    int server_port;
    struct hostent *hp;
    struct sockaddr_in sin;

    bzero((char*) &sin, sizeof(sin));
    sin.sin_family = AF_INET;

    //Abertura do socket
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("error: socket");
        return 0;
    }

    //Recuperando endereço do servidor
    char host[15+1]; //000.000.000.000\0
    for(i = 0; addr[i] != ':'; i++)
        host[i] = addr[i];
    host[i] = '\0';
    server_port = atoi(&addr[i+1]); //Lendo e convertendo a porta do servidor

    if(!(hp = gethostbyname(host))) {
        perror("error: gethostbyname");
        fprintf(stderr, "Host: %s\n", host);
        close(s);
        return 0;
    }

    bcopy(hp->h_addr, (char*) &sin.sin_addr, hp->h_length);
    sin.sin_port = htons(server_port);

    int len = sizeof(sin);
    if(connect(s, (struct sockaddr *) &sin, len) < 0) {
        perror("error: connect");
        close(s);
        return 0;
    }

    return s;
}

int main(int argc, char const *argv[]) {
    uint16_t myId;
    uint16_t sequ = 0;
    int s;

    //Enviar mensagem de OI para servidor:
    if(!(s = openSocket(argv[1])))
        exit(-1);

    puts("Sender: conectado");

    close(s);
    exit(0);
}