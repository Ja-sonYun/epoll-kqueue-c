#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "epoll.h"

#define BUF_SIZE 100
#define KEVENT_SIZE 50
void error_handling(char *buf);

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[BUF_SIZE];

    epoll(0, MULPLEX_INIT);
    int	event_cnt;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    epoll(serv_sock, MULPLEX_CREATE);

    while(1) {
        event_cnt = epoll(0, MULPLEX_WAIT);

        for (i=0; i<event_cnt; i++) {
	    int fd = epoll(i, MULPLEX_GET);
            if (fd == serv_sock) {
                adr_sz = sizeof(clnt_adr);
                clnt_sock= accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		epoll(clnt_sock, MULPLEX_CREATE);
                printf("connected client: %d \n", clnt_sock);

            } else {
                str_len = read(fd, buf, BUF_SIZE);
                if (str_len == 0) { // close request!
	            epoll(fd ,MULPLEX_CLOSE);
                    close(fd);
                    printf("closed client: %d \n", fd);
                } else {
					/* write(ep_events[i].data.fd, buf, str_len); */
                    write(fd, buf, str_len);    // echo!
                }
            }
        }
    }
 
    close(serv_sock);
    /* close(epfd); */
    return 0;
}

void error_handling(char *buf) {
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}
