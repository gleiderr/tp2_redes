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
    uint16_t myId;
    uint16_t sequ = 0;
    int s;

    if(!(s = openClient(argv[1])))
        exit(-1);

    sendMSG(s, OI, 0, SERVER_ID, sequ++, 0, NULL);
    recvData(s, (char*) &msg);
    if(msg.type == OK) {
        //Comunicação estabelecida!
        myId = msg.dest;
        printf("Hello, I'm Viewer %d.\n", myId);
    }

    int run = 1;
    while(run) {
        recvData(s, (char*) &msg);
        if(msg.dest == myId) {
            switch(msg.type) {
                case FLW:
                    sendMSG(s, OK, myId, msg.orig, msg.sequ, 0, NULL);
                    run = 0;
                    break;
                default:
                    fprintf(stderr, "error: msg.type(%d) inesperado\n", msg.type);
            }
        } //else ignora
    }
    
    close(s);
    printf("Viewer %d said goodbye!\n", myId);
    exit(0);
}