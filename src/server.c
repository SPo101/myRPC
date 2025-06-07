#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "parser.h"


int main(){

	config_server *server_settings = malloc(sizeof(config_server));
	int status = parse_conf_file("../config/server.conf", server_settings);
	if(status == -1){
		printf("Error reading a config file\n");
		exit(EXIT_FAILURE);
	}

	printf("server %s %s\n", server_settings->port, server_settings->socket);
	
	
	int sd, new_sd, pid, bytes_received;
	struct addrinfo hints, *res, *p;
	struct sockaddr_in *h, addr;
	socklen_t addr_size;
	char dest[INET_ADDRSTRLEN];
	char buffer[BUFFSIZE];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo(NULL, server_settings->port, &hints, &res);
	free(server_settings);

	for(p = res; p != NULL; p = p->ai_next){
		struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
		struct in_addr *addr = &(ipv4->sin_addr);
		inet_ntop(p->ai_family, addr, dest, sizeof dest);
		
		printf("%s\n", dest);
		
		if((sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("server: socket");
			continue;
		}
		if(bind(sd, p->ai_addr, p->ai_addrlen) == -1){
			close(sd);
			perror("server: bind");
			continue;
		}
		break;	
	}
	freeaddrinfo(res);
	
	listen(sd, BACKLOG);
	
	while(1){
		addr_size = sizeof addr;
		new_sd = accept(sd, (struct sockaddr *) &addr, &addr_size);
		if(new_sd == -1)
			continue;
		
		inet_ntop(addr.sin_family, &addr.sin_addr, dest, sizeof dest);
		printf("Got connection from: %s\n", dest);
		
		pid = fork();
		if(!pid){
			close(sd);
			user_data *u_data = malloc(sizeof(user_data));
			while(1){
				memset(buffer, 0, BUFFSIZE);
				bytes_received = recv(new_sd, buffer, BUFFSIZE, 0);	

				if(!strcmp(buffer, "Quit\n"))
					break;

				parse_user_input(buffer, u_data);						

				if((bytes_received) && (check_user(u_data->username) == SUCCESS))
					printf("\t[client %6d] - %s %s", getpid(), u_data->username, u_data->command);
			}
			free(u_data->username);
			free(u_data->command);
			free(u_data);
			close(new_sd);
			exit(EXIT_SUCCESS);
		}
		signal(SIGCHLD, SIG_IGN);	
		close(new_sd);
	}
	exit(EXIT_SUCCESS);
}
