#ifndef	__XSOCKET_H__
#define	__XSOCKET_H__ 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>


int xsocket_open(char *addr, int port, int type);
void xsocket_close(int sock);
int xsocket_listen(int sock);
int xsocket_accept(int sock, char *addr, int *port);
int xsocket_connect(int sock, char *addr, int port);
int xsocket_send(int sock, unsigned char *buf, int len);
int xsocket_sendto(int sock, char *addr, int port, unsigned char *buf, int len);
int xsocket_recv(int sock, unsigned char *buf, int len);
int xsocket_recvfrom(int sock, char *addr, int *port, unsigned char *buf, int len);
int xsocket_poll(int sock, int timeout);


#endif
