#include <stdio.h>
#include "xsocket.h"

#define		BUF_SIZE	1024

char *addr = "127.0.0.1";
int port = 0;

int sock = -1;
int client = -1;

void disconnect_client()
{
    if(client >= 0){
	printf("Client disconnected\n");
	close(client);
    }
    client = -1;
}


int main(int argc, char **argv)
{
    char cl_addr[32];
    int cl_port;
    int len;
    char buf[BUF_SIZE];
    int ret;

    if(argc > 1)
	port = atoi(argv[1]);
    if(argc > 2)
	addr = strdup(argv[2]);

    if(port == 0){
	printf("usage:\n\t/client port addr [laddr] [lport]\n");
	return 1;
    }

    printf("Openning %s:%d\n", addr, port);
    sock = xsocket_open(addr, port, SOCK_STREAM);
    if(sock < 0)
	return 2;

    xsocket_listen(sock);

    while(1){
	ret = xsocket_poll(sock, 100);

	if(ret < 0){
	    printf("Error on server sock\n");
	    break;
	}

	if(ret > 0){
	    disconnect_client();
	    client = xsocket_accept(sock, cl_addr, &cl_port);
	    if(client >= 0)
		printf("Client(%s:%d) connected\n", cl_addr, cl_port);
	}

	if(client>=0 ) {
	    ret = xsocket_poll(client, 100);
	    if(ret < 0 ){
		// error happened
		disconnect_client();
	    }
	    if(ret > 0){
		// data ready
		len = xsocket_recv(client, buf, BUF_SIZE);
		if(len > 0)
		    xsocket_send(client, buf, len);
	    }
	}
    }
    return 0;
}
