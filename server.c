/**
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 *
 * Copyright 2012 by Gabriel Parmer.
 * Author: Gabriel Parmer, gparmer@gwu.edu, 2012
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>

/* 
 * Create the file descriptor to accept on.  Return -1 otherwise.
 */
int 
server_create(short int port)
{
	int fd;
	struct sockaddr_in server;

	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Establishing socket");
		return -1;
	}

	server.sin_family      = AF_INET;
	server.sin_port        = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(fd, (struct sockaddr *)&server, sizeof(server))) {
		perror("binding receive socket");
		return -1;
	}
	listen(fd, 10);

	return fd;
}


/* 
 * Pass in the accept file descriptor returned from
 * server_create. Return a new file descriptor or -1 on error.
 */
int 
server_accept(int fd)
{
	struct sockaddr_in sai;
	int new_fd;
	unsigned int len = sizeof(sai);

	new_fd = accept(fd, (struct sockaddr *)&sai, &len);
	if (-1 == new_fd) {
		perror("accept");
		return -1;
	}
	return new_fd;
}

int
client_create(char *addr, int port)
{
   	struct sockaddr_in serv_addr; 

   	int fd;

    	if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{
        	perror("Error : Could not create socket");
        	return -1;
    	} 

    	memset(&serv_addr, '0', sizeof(serv_addr)); 

    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(port); 

    	if(inet_pton(AF_INET, addr, &serv_addr.sin_addr)<=0)
   	{
        	perror("inet_pton error occured");
        	return -1;
    	} 

    	if( connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	{
       		perror("Error : Connect Failed");
       		return -1;
    	} 

	return fd;
}

int
epoll_add_fd(int efd, int fd)
{
	struct epoll_event event;

	event.data.fd = fd;
        event.events = EPOLLIN | EPOLLET;
	int s = epoll_ctl(efd, EPOLL_CTL_ADD, fd, &event);
	if (s == -1)
    	{
     		printf("epoll_ctl error \n");
      		return -1;
    	}
	return 0;
}



