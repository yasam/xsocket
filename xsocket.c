#include <errno.h>
#include "xsocket.h"

int xsocket_open(char *addr, int port, int type)
{
	int fd;
	struct sockaddr_in myaddr;

	memset((char *)&myaddr, 0, sizeof(myaddr));

	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr((char *)addr);
	myaddr.sin_port = htons(port);


	if ((fd = socket(AF_INET, type, 0)) < 0) {
		perror("cannot create socket");
		return -1;
	}

	//printf("binding:%s:%d\n", addr,port);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed:");
		close(fd);
		return -1;
	}

	return fd;
}
void xsocket_close(int sock)
{
	close(sock);
}
int xsocket_listen(int sock)
{
    return listen(sock, 100);
}
int xsocket_accept(int sock, char *addr, int *port)
{
	int fd;
	struct sockaddr_in rem;
	socklen_t slen = sizeof(rem);

	memset((char *)&rem, 0, sizeof(rem));

	fd = accept(sock, (struct sockaddr *)&rem, &slen);
	if(fd < 0){
		perror("accept failed:");
		return -1;
	}

	if(port != NULL)
	    *port = ntohs(rem.sin_port);

	if(addr != NULL)
	    strcpy((char *)addr, inet_ntoa(rem.sin_addr));

	return fd;
}


int xsocket_connect(int sock, char *addr, int port)
{
	struct sockaddr_in rem;

	memset((char *)&rem, 0, sizeof(rem));

	rem.sin_family = AF_INET;
	rem.sin_addr.s_addr = inet_addr((char *)addr);
	rem.sin_port = htons(port);

	if(connect(sock, (struct sockaddr*)&rem, sizeof(rem))!=0){
		perror("connect failed:");
		return -1;
	}

	return 0;
}

int xsocket_send(int sock, unsigned char *buf, int len)
{
	if (send(sock, buf, len, MSG_NOSIGNAL) < 0) {
		perror("send failed");
		return 0;
	}
	return len;
}

int xsocket_sendto(int sock, char *addr, int port, unsigned char *buf, int len)
{
	struct sockaddr_in remaddr;

	memset((char *)&remaddr, 0, sizeof(remaddr));
	remaddr.sin_port = htons(port);
	remaddr.sin_addr.s_addr = inet_addr(addr);

	if (sendto(sock, buf, len, MSG_NOSIGNAL, (struct sockaddr *)&remaddr, sizeof(remaddr)) < 0) {
		perror("sendto failed");
		return 0;
	}
	return len;
}
int xsocket_recv(int sock, unsigned char *buf, int len)
{
	int recvlen;

	recvlen = recv(sock, buf, len, 0);

	if(recvlen <= 0)
		return 0;

	return recvlen;
}

int xsocket_recvfrom(int sock, char *addr, int *port, unsigned char *buf, int len)
{
	struct sockaddr_in remaddr;
	socklen_t addrlen = sizeof(remaddr);
	int recvlen;

	recvlen = recvfrom(sock, buf, len, 0, (struct sockaddr *)&remaddr, &addrlen);

	if(recvlen <= 0)
		return 0;

	strcpy((char *)addr, inet_ntoa(remaddr.sin_addr));
	*port = ntohs(remaddr.sin_port);

	return recvlen;
}

int xsocket_poll(int sock, int timeout)
{
	struct pollfd pfd[1];
	int result;
	
	pfd[0].fd = sock;
	pfd[0].events = POLLIN | POLLERR | POLLHUP | POLLNVAL;
	pfd[0].revents = 0;

	result = poll(pfd, 1, timeout);

	if(result < 0 && errno == EINTR)
	    return 0;
	
	if(result == 0)
	    return 0;
	
	if(pfd[0].revents & (POLLERR | POLLHUP | POLLNVAL))
	    return -1;

	if(pfd[0].revents & POLLIN)
	    return 1;

	return 0;
}
