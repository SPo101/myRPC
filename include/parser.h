#ifndef PARSER
#define PARSER


typedef struct {
	char *port;
	char *socket;
}config_server;

typedef struct {
	char *username;
	char *command;
}user_data;

#define SUCCESS 0
#define FAILURE -1
#define BACKLOG 5
#define BUFFSIZE 1024


int parse_conf_file(const char *path, config_server *settings);

void parse_user_input(const char *buffer, user_data *u_data);

int check_user(const char *user);

#endif
