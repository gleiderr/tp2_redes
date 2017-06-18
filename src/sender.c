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
#include "clientConnection.h"

int main(int argc, char const *argv[]) {
    Mensagem msg;
    uint16_t myId, myViewer;

    uint16_t sequ = 0;
    int s;

    //Enviar mensagem de OI para servidor:
    if(!(s = openClient(argv[1])))
        exit(-1);

    myViewer = argc == 3 ? atoi(argv[2]) : 1;

    sendMSG(s, OI, myViewer, SERVER_ID, sequ++, 0, NULL);
    recvData(s, (char*) &msg);
    if(msg.type == OK) {
        //Comunicação estabelecida!
        myId = msg.dest;
        printf("Hello, I'm Sender %d.\n", myId);
    }

    int run = 1, waitOK = 0;
    char buff[UINT16_MAX];
    while(run) {
        printf("$ ");
        fgets(buff, UINT16_MAX, stdin);

        if(strcmp(buff, "flw\n") == 0) {
            //Encerrar esse sender. Servidor encerra o viewer deste sender
            sendMSG(s, FLW, myId, SERVER_ID, sequ++, 0, NULL);
            run = 0;
            waitOK = 1;
        }
    }

    while(waitOK) {
        recvData(s, (char*) &msg);
        if(msg.type == OK && msg.orig == SERVER_ID && msg.dest == myId && msg.sequ == (sequ-1))
            waitOK = 0;
    }

    close(s);
    printf("Sender %d said goodbye!\n", myId);
    exit(0);
}