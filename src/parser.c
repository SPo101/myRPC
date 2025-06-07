#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#include "parser.h"

int find_word_place(const char *text, const char *right_word, const char *escape_word){
	const char *port = right_word;
	const char *wport = escape_word;
	char *pos = strstr(text, port);
	char *wpos = strstr(text, wport);

	if(pos == NULL)
		return FAILURE;
	
	while( (pos-1 == wpos)){
		pos = strstr(pos+1, port);
		wpos = strstr(wpos+1, wport);
	}

	if(pos == NULL)
		return FAILURE;

	pos -= text;
	return pos;
}

int read_data(const char *text, char **settings_part, const char *right_word, const char *escape_word){
	int line_start = find_word_place(text, right_word, escape_word);

	if(line_start == -1)
		return FAILURE;

	
	char *end_line = strchr(text+line_start, '\n');
	char *word_start = strchr(text+line_start, '=');

	if((word_start == NULL) || (word_start > end_line))
		return FAILURE;

	*settings_part = malloc(end_line-word_start);
	strncpy(*settings_part, word_start+2, end_line-word_start-2);

	return SUCCESS;
}

int parse_conf_file(const char *path, config_server *settings){
	int fd = open(path, O_RDONLY);
	int offset = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	char *text = malloc(offset);
	read(fd, text, offset);
	
	if(read_data(text, &settings->port, "port", "#port") == FAILURE)
		return FAILURE;
	if(read_data(text, &settings->socket, "socket_type", "#socket_type") == FAILURE)
		return FAILURE;


	free(text);
	return SUCCESS;
}



void parse_user_input(const char *buffer, user_data *u_data){

				int mid;
				for(int i=0; i<BUFFSIZE; i++)
					if(buffer[i] == '|'){
						mid = i;
						break;
					}
				u_data->username = malloc(mid);	
				u_data->command  = malloc(BUFFSIZE-mid);
				strncpy(u_data->username, buffer, mid);
				strncpy(u_data->command, buffer+mid+1, BUFFSIZE-mid);

}

int check_user(const char *user){
	int fd = open("../config/users.conf", O_RDONLY);
	int offset = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	char *text = malloc(offset);
	read(fd, text, offset);
	
	char *result = strstr(text, user);
	if(result != NULL)
		return SUCCESS;
	return FAILURE;
}

