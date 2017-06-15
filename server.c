#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "mensagem.h"

#define MAX_PENDING 256

#define FRST_SENDER 1
#define LAST_SENDER (pow(2, 12) - 1)
#define FRST_VIEWER (pow(2, 12))
#define LAST_VIEWER (pow(2, 13) - 1)


//Estruturas de dados de clientes
typedef struct {
    int s;
} Client;
#define MAX_CLIENTS 256;
int nClients = 0;
Client clients[256];

int passive_s; //Descritor para novas conexões

int openSocket(char const* addr, struct sockaddr_in* sin) {
    int server_port;
    if ((passive_s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("error: socket");
        return 0;
    }

    server_port = atoi(addr); //Lendo e convertendo a porta do servidor
    bzero((char*) sin, sizeof(struct sockaddr_in));
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = INADDR_ANY;
    sin->sin_port = htons(server_port);

    if(bind(passive_s, (struct sockaddr*) sin, sizeof(struct sockaddr_in)) < 0) {
        perror("error: bind");
        close(passive_s);
        return 0;
    }
    
    if(listen(passive_s, MAX_PENDING) < 0) {
        perror("error: listen");
        close(passive_s);
        return 0;   
    }
    return 1;
}

/* Função para atribuição de novos clientes */
void newClient(struct sockaddr_in* sin) {
    if(nClients == 256) {
        fprintf(stderr, "error: Máximo de clients atingido\n");
        return;
    }

    int len = sizeof(struct sockaddr_in);
    int new_s;
    if((new_s = accept(passive_s, (struct sockaddr*) sin, (socklen_t*) &len)) < 0) {
        perror("error: accept");
        close(passive_s);
        exit(-1);
    }

    clients[nClients].s = new_s;
    nClients++;
}

int main(int argc, char const *argv[]) {
    int i;
    struct sockaddr_in sin;
    uint16_t myId = SERVER_ID;
    uint16_t nextSender = FRST_SENDER;
    uint16_t nextViewer = FRST_VIEWER;
    uint16_t sequ = 0;

    fd_set rfds;
    int nfds;

    if(!openSocket(argv[1], &sin))
        exit(-1);

    //Reiniciando rfds e nfds
    FD_ZERO(&rfds);
    FD_SET(passive_s, &rfds);
    nfds = passive_s + 1;
    for(i = 0; i < nClients; i++){
        FD_SET(clients[i].s, &rfds);
        if(clients[i].s >= nfds)
            nfds = clients[i].s + 1;
    }

    //Selecionando descritor de arquivo pronto para ser lido
    if(select(nfds, &rfds, NULL, NULL, NULL) < 0) {
        perror("error: select");
    } else {
        //Nova conexão à vista
        if(FD_ISSET(passive_s, &rfds)){
            newClient(&sin);
            puts("Server: conectado");
        }

        //Para cada descritor pronto, receber seus dados
    }

    //Fechando sockets
    for(i = 0; i < nClients; i++)
        close(clients[i].s);
    close(passive_s);
    exit(0);
}