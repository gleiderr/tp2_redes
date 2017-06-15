#include <sys/select.h>
#include <sys/socket.h>

#include "cabecalho.h"

#define FRST_SENDER 1
#define LAST_SENDER (pow(2, 12) - 1)
#define FRST_VIEWER (pow(2, 12))
#define LAST_VIEWER (pow(2, 13) - 1)

int openSocket(char const* addr) {
    int s;
    int server_port;
    struct sockaddr_in sin;

    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("error: socket");
        return 0;
    }

    server_port = atoi(addr); //Lendo e convertendo a porta do servidor
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);

    if(bind(s, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
        perror("error: bind");
        close(s);
        return 0;
    }
    listen(s, MAX_PENDING);
    if(bind(s, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
        perror("error: bind");
        close(s);
        return 0;
    }
    int other_s;
    int len = sizeof(sin);
    if((other_s = accept(s, (struct sockaddr*) &sin, (socklen_t*) &len)) < 0) {
        perror("error: accept");
        close(s);
        return 0;
    }
    return other_s;
}

int main(int argc, char const *argv[]) {   
    uint16_t myId = pow(2, 16) - 1;
    uint16_t nextSender = FRST_SENDER;
    uint16_t nextViewer = FRST_VIEWER;
    uint16_t sequ = 0;

    int s;
    fd_set rfds;

    if(!(s = openSocket(argv[1])))
        exit(-1);

    exit(0);
}