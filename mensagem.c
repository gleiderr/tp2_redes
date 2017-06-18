#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "mensagem.h"

int openClient(char const* addr) {
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

void sendMSG(int s, uint16_t type, uint16_t orig, uint16_t dest, uint16_t sequ, uint16_t length, char* msg) {
    //printf("sendMSG(s(%d), type(%d), orig(%d), dest(%d), sequ(%d), length(%d))\n", s, type, orig, dest, sequ, length);
    Mensagem m;
    m.type = type;
    m.orig = orig;
    m.dest = dest;
    m.sequ = sequ;
    m.length = length;
    memcpy(m.msg, msg, length);
    
    if(send(s, &m, 10 + m.length, 0) < 0){
        perror("error: sendMSG");
        close(s);
        exit(1);
    }
}

void recvData(int s, char* buff) {
    Mensagem* msg = (Mensagem*) buff;

    //printf("recvData_in(s(%d), buff(%p))\n", s, buff);

    //Recebimento do cabeçalho
    if(recv(s, buff, 10, 0) < 0)
        perror("error: recv1");

    if(msg->type == MSG) {
        if(recv(s, &buff[10], msg->length, 0) < 0)
            perror("error: recv2");
    }

    //printf("recvData_out()\n");
}