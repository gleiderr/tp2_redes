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

    //bool flagEND = FALSE;

    //Enviar mensagem de OI para servidor:
    if(!(s = openClient(argv[1])))
        exit(-1);

    //puts("Sender: conectado");
    sendMSG(s, OI, 1, SERVER_ID, sequ++, 0, NULL);
    recvData(s, (char*) &msg);
    if(msg.type == OK) {
        //Comunicação estabelecida!
        myId = msg.dest;
        printf("Hello, I'm Sender %d.\n", myId);
    }

  /** Só comentei porque na resolução de conflitos eu não entendi esse trecho :/
	while(myId == 0)
	{
		waitforId();
	}
  */

    close(s);
    exit(0);
}