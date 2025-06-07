#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <dlfcn.h>
#include <stddef.h>





#define CNT_OPT 6
#define SUCCESS 0
#define FAILURE -1

typedef struct {
	char *command;
	char *host;
	char *port;
	int stream;
	int dgram;
}config_client;

void usage(){
	printf("Usage\n");
	printf("-c --command	Command which will be sent to server\n");
	printf("-h --host	Server name\n");
	printf("-p --port	Server port\n");
	printf("-s --stream	Use stream socket(tcp)\n");
	printf("-d --dgram	Use dgram socket(udp)\n");
	printf("   --help	Help\n");
	
	exit(EXIT_SUCCESS);
}

int read_client_args(int cnt_args, char *args[], config_client *client_settings){

	int Option = 0; 
	int Option_index = 0;


	static struct option Long_options[] = {
		{"command", 	required_argument, 	0, 'c'},
		{"host", 	required_argument, 	0, 'h'},
		{"port",  	required_argument, 	0, 'p'},
		{"stream", 	no_argument,		0, 's'},
		{"dgram", 	no_argument, 		0, 'd'},
		{"help", 	no_argument, 		0,  0}
	};
	static char *Short_options = "c:h:p:sdh";

	while(1){
		Option = getopt_long(cnt_args, args, Short_options, Long_options, &Option_index);
		if(Option == -1)
			break;
		
		switch(Option){
			case 0:
				usage();
				break;
			case 'c':
				client_settings->command = optarg;
				break;
			case 'h':
				client_settings->host = optarg;
				break;
			case 'p':
				client_settings->port = optarg;
				break;
			case 's':
				client_settings->stream = 1;
				client_settings->dgram = 0;
				break;
			case 'd':
				client_settings->stream = 0;
				client_settings->dgram = 1;
				break;

			case '?':
				break;
		}
	}

	
	if((client_settings->command == NULL)||(client_settings->host == NULL)||(client_settings->port == NULL))
		return FAILURE;
	if(!(client_settings->stream + client_settings->dgram))
		return FAILURE;

	return SUCCESS;
}

int main(int argc, char *argv[]){

	if(argc < 2){
		fprintf(stderr, "Not all parameters were given\n");
		usage();
	}

	int status;

	config_client *client_settings = malloc(sizeof(config_client));
	status = read_client_args(argc, argv, client_settings);
	if(status == FAILURE)
		usage();

	printf("%s %s %s\n", client_settings->command, client_settings->host, client_settings->port);
	printf("%d %d\n", client_settings->stream, client_settings->dgram);

	free(client_settings);
}

	
