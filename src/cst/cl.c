#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "8080"
#define BUFFSIZE 1024

int main(int argc, char **argv){
	
	int status, sd;
	struct addrinfo hints, *res, *p;
	char buffer[BUFFSIZE];
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(argv[1], argv[2], &hints, &res);
	
	for(p = res; p != NULL; p = p->ai_next){
		if((sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("client: socket");
			continue;
		}

		if(connect(sd, res->ai_addr, res->ai_addrlen) == -1){
			close(sd);
			perror("client: connect");
			continue;
		}
		break;
	}
	
	while(1){
		memset(buffer, 0, BUFFSIZE);
		read(0, buffer, BUFFSIZE);
		send(sd, buffer, BUFFSIZE, 0);
		if(!strcmp(buffer, "Quit\n"))
			break;
	}

	freeaddrinfo(res);
	close(sd);
	exit(EXIT_SUCCESS);
}
