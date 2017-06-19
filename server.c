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

int passive_s; //Descritor para novas conexões
fd_set rfds_bkp; //Conjunto de descritores ativos

//Estruturas de dados de clientes
typedef struct {
    int id;
    int viewer;
} Client;
int nClients = 0;
Client clients[FD_SETSIZE]; //índice = socket do cliente

int client_s(int id) {
    int s;
    for(s = 0; s < FD_SETSIZE; s++)
        if(FD_ISSET(s, &rfds_bkp) && clients[s].id == id)
            return s; //Retorna socket

    return -1; //Se não encontrar retorna -1
}

/* Função para atribuição de novos clientes */
int newClient(struct sockaddr_in* sin) {
    if(nClients == FD_SETSIZE) {
        fprintf(stderr, "error: Máximo de clients atingido: %d\n", FD_SETSIZE);
        return -1;
    }

    int new_s;
    int len = sizeof(struct sockaddr_in);
    if((new_s = accept(passive_s, (struct sockaddr*) sin, (socklen_t*) &len)) < 0) {
        perror("error: accept");
        close(passive_s);
        exit(-1);
    }

    nClients++;
    //printf("New client #%d socket: %d.\n", nClients, new_s);
    return new_s;
}

void disconnect(int s) {
    FD_CLR(s, &rfds_bkp);
    close(s);
    nClients--;
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

    return 1;
}

uint16_t nextSender = FRST_SENDER;
uint16_t nextViewer = FRST_VIEWER;
void processData(int s) {
    //puts("processData()");
    Mensagem msg;

    recvData(s, (char*) &msg);
    switch(msg.type) {
        case OK:
            break;
        case ERRO:
            break;
        case OI:
            if(msg.orig == 0) { //Exibidor
                clients[s].id = nextViewer++;
                clients[s].viewer = 0;
            } else if(msg.orig >= FRST_VIEWER && msg.orig <= LAST_VIEWER) { //Emissor com viewer
                clients[s].id = nextSender++;
                clients[s].viewer = msg.orig;
            } else { //Emissor sem viewer
                clients[s].id = nextSender++;
                clients[s].viewer = 0;
            }
            sendMSG(s, OK, SERVER_ID, clients[s].id, msg.sequ, 0, NULL);
            break;
        case FLW:
            if(msg.orig >= FRST_SENDER && msg.orig <= LAST_SENDER) {//FLW de emissor
                if(msg.orig == clients[s].id) { //Confirmando identidade do emissor!
                    //FLW para viewer associado
                    if(clients[s].viewer) {
                        int s_viewer = client_s(clients[s].viewer);
                        sendMSG(s_viewer, FLW, SERVER_ID, clients[s].viewer, msg.sequ/*Qual seria?*/, 0, NULL);

                        disconnect(s_viewer); //Desconecta viewer
                    }

                    //Desconecta sender
                    sendMSG(s, OK, SERVER_ID, clients[s].id, msg.sequ, 0, NULL);
                    disconnect(s);
                } //else, simplesmente ignora
            }
            break;
        case MSG:
            int s_dest;
            if(msg.orig == clients[s].id) { //Confirmando identidade do emissor!
                if(msg.dest) {
                    s_dest = client_s(msg.dest);

                    //Se destino for sender, destino é o viewer dele
                    if(clients[s_dest].id >= FRST_SENDER && clients[s_dest].id <= LAST_SENDER){
                        if(clients[s_dest].viewer) {
                            s_dest = client_s(clients[s_dest].viewer);
                        } else { //Erro se não houver viewer associando ao sender destinatário
                            s_dest = -1;
                        }
                    }
                    if(s_dest >= 0){
                        sendMSG(s_dest,msg.type,msg.orig, msg.dest, msg.sequ, msg.length, msg.msg);
                        sendMSG(s, OK, SERVER_ID, msg.orig, msg.sequ, 0, NULL);
                    } else {
                        sendMSG(s, ERRO, SERVER_ID, clients[s].id, msg.sequ, 0, NULL);
                    }
                } else { //broadcast para todos viewers
                    for(s_dest = 0; s_dest < FD_SETSIZE; s_dest++) {
                        if(FD_ISSET(s_dest, &rfds_bkp)) {
                            int id = clients[s_dest].id;
                            if(id >= FRST_VIEWER && id <= LAST_VIEWER) //Se viewer
                                sendMSG(s_dest, msg.type, msg.orig, msg.dest, msg.sequ, msg.length, msg.msg);
                        }
                    }
                    sendMSG(s, OK, SERVER_ID, msg.orig, msg.sequ, 0, NULL);
                }
            }
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
    int s, j;
    struct sockaddr_in sin;
    uint16_t sequ = 0;

    fd_set rfds;

    if(!openSocket(argv[1], &sin))
        exit(-1);

    //processData(&clients[0]);
    FD_ZERO(&rfds_bkp);
    FD_SET(passive_s, &rfds_bkp);

    int run = 1;
    while(run) {
        rfds = rfds_bkp;
        /*for(s = 0; s < FD_SETSIZE; s++)
            if(FD_ISSET(s, &rfds))
                printf("Socket conectado: %d.\n", s);*/

        //Selecionando descritor de arquivo pronto para ser lido
        //printf("select_in()\n");
        if(select(FD_SETSIZE, &rfds, NULL, NULL, NULL) < 0) {
            perror("error: select");
            exit(-1);
        } else {
            //printf("select_out()\n");
            for(s = 0; s < FD_SETSIZE; s++) { //Testa todos possíveis sockets remetentes
                //printf("FD_ISSET(s(%d/%d), &rfds): %d.\n", s, FD_SETSIZE, FD_ISSET(s, &rfds));
                if(FD_ISSET(s, &rfds)) {
                    if(s == passive_s) {//Novo cliente
                        int new_s = newClient(&sin);
                        FD_SET(new_s, &rfds_bkp);
                    } else //Dados chegando
                        processData(s);
                }
            }
        }
    }

    //Fechando sockets ainda abertos
    for(s = 0; s < FD_SETSIZE; s++)
        if(FD_ISSET(s, &rfds_bkp))
            close(s);
    exit(0);
}