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

sendOI(int s, int sequ) {
    Mensagem m;
    m.type = OI;
    m.orig = 0;
    m.dest = SERVER_ID;
    m.sequ = sequ;
}

int main(int argc, char const *argv[]) {   
    uint16_t myId;
    uint16_t sequ = 0;
    int s;

    if(!(s = openClient(argv[1])))
        exit(-1);

    puts("Viewer: conectado");
    sendOI(s, ++sequ);
    
    close(s);
    exit(0);
}