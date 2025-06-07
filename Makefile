CC := /usr/bin/gcc
Flags := -I ./include
IDir := ./include/
SDir := ./src/
BDir := ./.build/
Client := client username
Server := server parser libmysyslog

SClient = $(addprefix $(SDir), $(Client:%=%.c))
SServer = $(addprefix $(SDir), $(Server:%=%.c))


all: create_dir $(BDir)Client $(BDir)Server mysyslog_lib


$(BDir)Client: $(SClient)
	@$(CC) $(Flags) -o $@ $^

$(BDir)Server: $(SServer)
	@$(CC) $(Flags) -o $@ $^

mysyslog_lib:
	@$(CC) $(SDir)libmysyslog-text.c -shared -o $(BDir)libmysyslog-text.so
	@$(CC) $(SDir)libmysyslog-json.c -shared -o $(BDir)libmysyslog-json.so

create_dir:
	@./create_dir.sh

clean:
	@rm -rf .build
	@rm -f $(SDir)libmysyslog-text.so
	@rm -f $(SDir)libmysyslog-json.so

