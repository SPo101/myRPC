#ifndef PARSER
#define PARSER


typedef struct {
	char *port;
	char *socket;
}config_server;

#define SUCCESS 0
#define FAILURE -1


int parse_conf_file(const char *path, config_server *settings);

#endif
