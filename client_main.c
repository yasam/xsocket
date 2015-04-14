#include <stdio.h>
#include "xsocket.h"

#define		BUF_SIZE	1024

char *laddr="0.0.0.0";
int lport = 0;
char *addr = "127.0.0.1";
int port = 0;

int sock = -1;


int main(int argc, char **argv)
{
    int len;
    char buf[BUF_SIZE];
    int ret;

    if(argc > 1)
	port = atoi(argv[1]);
    if(argc > 2)
	addr = strdup(argv[2]);

    if(argc > 3)
	lport = atoi(argv[3]);
    if(argc > 4)
	laddr = strdup(argv[4]);


    if(port == 0){
	printf("usage:\n\t/echod port [addr]\n");
	return 1;
    }

    printf("Openning %s:%d\n", addr, port);
    sock = xsocket_open(laddr, lport, SOCK_STREAM);
    if(sock < 0)
	return 2;

    if(xsocket_connect(sock,addr, port)<0)
	return 3;

    while(1){
	if(xsocket_poll(STDIN_FILENO, 100)){
	    len = read(STDIN_FILENO, buf, BUF_SIZE);
	    if(len > 0)
		xsocket_send(sock, buf, len);
	}

	ret = xsocket_poll(sock, 100);
	if(ret < 0){
	    printf("Connection err\n");
	    break;
	}
	if(ret > 0){
	    len = xsocket_recv(sock, buf, BUF_SIZE);
	    if(len > 0)
		write(STDOUT_FILENO, buf, len);
	}
    }
    return 0;
}
