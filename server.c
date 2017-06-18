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
#define LAST_SENDER 4095
#define FRST_VIEWER 4096
#define LAST_VIEWER 8191


//Estruturas de dados de clientes
typedef struct {
    int s;
    int id;
    int viewer;
} Client;
#define MAX_CLIENTS 256;
int nClients = 0;
Client clients[256];

int passive_s; //Descritor para novas conexões

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

    newClient(sin);
    puts("Server: conectado");

    return 1;
}

int resetFDS(fd_set* fds) {
    int i;
    int nfds = passive_s + 1;

    FD_ZERO(fds);
    FD_SET(passive_s, fds);
    for(i = 0; i < nClients; i++){
        FD_SET(clients[i].s, fds);
        if(clients[i].s >= nfds)
            nfds = clients[i].s + 1;
    }

    return nfds;
}

uint16_t nextSender = FRST_SENDER;
uint16_t nextViewer = FRST_VIEWER;
void processData(Client* client) {
    puts("processData()");
    Mensagem msg;
    int rtn = 0, r;

    recvData(client->s, (char*) &msg);
    switch(msg.type) {
        case OK:
            break;
        case ERRO:
            break;
        case OI:
            if(msg.orig == 0) { //Exibidor
                client->id = nextViewer++;
                client->viewer = 0;
            } else if(msg.orig >= FRST_VIEWER || msg.orig <= LAST_VIEWER) { //Emissor
                client->id = nextSender++;
                client->viewer = msg.orig;
            } else {
                client->id = nextSender++;
                client->viewer = 0;
            }
            sendMSG(client->s, OK, SERVER_ID, client->id, 0, 0, NULL);
            break;
        case FLW:
            break;
        case MSG:
            break;
        case CREQ:
            break;
        case CLIST:
            break;
        default:
            fprintf(stderr, "error: msg.type(%d) inválido\n", msg.type);
    }
}

int main(int argc, char const *argv[]) {
    int i, j;
    struct sockaddr_in sin;
    uint16_t sequ = 0;

    int nfds, n;
    fd_set rfds, rfds_bkp;

    if(!openSocket(argv[1], &sin))
        exit(-1);

    processData(&clients[0]);

    /*int run = 1;
    while(run) {
        nfds = passive_s + 1;
        FD_ZERO(&rfds);
        FD_SET(passive_s, &rfds);
        for(i = 0; i < nClients; i++){
            printf("clients[i].s(%d)\n", clients[i].s);
            FD_SET(clients[i].s, &rfds);
            if(clients[i].s >= nfds)
                nfds = clients[i].s + 1;
        }
        //Selecionando descritor de arquivo pronto para ser lido
        //int n, nfds = resetFDS(&rfds);
        puts("select_in()");
        if(n = select(FD_SETSIZE, &rfds, NULL, NULL, NULL) < 0) {
            perror("error: select");
            exit(-1);
        } else {
            printf("select_out(); n=%d;\n", n);
            //Nova conexão à vista
            if(FD_ISSET(passive_s, &rfds)){
                n--;
                newClient(&sin);
            }

            //Para cada descritor pronto, receber seus dados

            for(i = 0, j = 0; i < n; i++) {
                while(!FD_ISSET(clients[j].s, &rfds)) j++; //Seleciona cliente entregando mensagem
                processData(&clients[j]);
            }
        }
    }*/

    //Fechando sockets
    for(i = 0; i < nClients; i++)
        close(clients[i].s);
    close(passive_s);
    exit(0);
}