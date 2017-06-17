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
    uint16_t myId;
    uint16_t sequ = 0;
    int s;

    if(!(s = openClient(argv[1])))
        exit(-1);

    puts("Viewer: conectado");
    sendMSG(s, OI, 0, SERVER_ID, ++sequ);
    
    close(s);
    exit(0);
}