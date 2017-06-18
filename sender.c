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
    uint16_t myId, myViewer, dest; //identificadores

    uint16_t sequ = 0;
    int s;
    int i;

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

    //Breve manual do usuário
    puts(">> To exit, type "flw";");
    puts(">> To send a message to someone with id 'n' type:");
    puts("   $ n, this is my message!");
    puts(">> To broadcast type:");
    puts("   $ 0, this is my message!\n");

    int run = 1, waitOK = 0;
    char buff[UINT16_MAX];
    while(run) {
        printf("$ ");
        fgets(buff, UINT16_MAX, stdin);

        if(strcmp(buff, "flw\n") == 0) {
            //Encerrar esse sender. Servidor encerra o viewer deste sender
            sendMSG(s, FLW, myId, SERVER_ID, sequ++, 0, NULL);
            wait(OK, SERVER_ID, myId, sequ-1);
            run = 0;
        } else { //Mensagem MSG

            //Identificação do destino
            char aux[10];
            for(i = 0; buff[i] != ','; i++)
                aux[i] = buff[i];
            aux[i] = '\0';
            dest = atoi(aux);

            //Identificação do início da mensagem
            while(buff[i] == ',' || buff[i] == ' ')
                i++;

            sendMSG(s, MSG, myId, dest, sequ++, strlen(&buff[i]), &buff[i]);
            wait(OK, SERVER_ID, myId, sequ-1);
        }
    }

    close(s);
    printf("Sender %d said goodbye!\n", myId);
    exit(0);
}