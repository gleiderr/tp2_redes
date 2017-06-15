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

int openSocket(char const* addr);

int main(int argc, char const *argv[]) {   
    uint16_t myId;
    uint16_t sequ = 0;
    int s;

    //Enviar mensagem de OI para servidor:
    
    close(s);
    exit(0);
}