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

int main(int argc, char const *argv[]) {
    Mensagem msg;
    uint16_t myId;
    uint16_t sequ = 0;
    int s;

    if(!(s = openClient(argv[1])))
        exit(-1);

    //puts("Viewer: conectado");
    sendMSG(s, OI, 0, SERVER_ID, sequ++, 0, NULL);
    recvData(s, (char*) &msg);
    if(msg.type == OK) {
        //Comunicação estabelecida!
        myId = msg.dest;
        printf("Hello, I'm Viewer %d.\n", myId);
    }
    
    close(s);
    exit(0);
}