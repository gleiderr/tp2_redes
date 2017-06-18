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

int main(int argc, char const *argv[]) 
{
    uint16_t myId = 0;
    uint16_t sequ = 0;
    int s;

    bool flagEND = FALSE;

    //Enviar mensagem de OI para servidor:
    if(!(s = openClient(argv[1])))
        exit(-1);
	puts("Sender: conectado");

	/* Enviando OI para requisição.  */
	void sendOI(s, sequ);

	while(myId == 0)
	{
		waitforId();
	}

    close(s);
    exit(0);
}