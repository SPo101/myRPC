CC := /usr/bin/gcc
Flags := -I ./include
IDir := ./include/
SDir := ./src/
BDir := ./.build/
Client := client username
Server := server parser

SClient = $(addprefix $(SDir), $(Client:%=%.c))
SServer = $(addprefix $(SDir), $(Server:%=%.c))


all: create_dir $(BDir)Client $(BDir)Server


$(BDir)Client: $(SClient)
	@$(CC) $(Flags) -o $@ $^

$(BDir)Server: $(SServer)
	@$(CC) $(Flags) -o $@ $^


create_dir:
	@./create_dir.sh

clean:
	@rm -rf .build

