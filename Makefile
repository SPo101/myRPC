CC := /usr/bin/gcc
Flags := -I ./include
IDir := ./include/
SDir := ./src/
BDir := ./.build/
Files := client username

SFiles = $(addprefix $(SDir), $(Files:%=%.c))


all: create_dir $(BDir)Files



$(BDir)Files: $(SFiles)
	@$(CC) $(Flags) -o $@ $^


create_dir:
	@./create_dir.sh

clean:
	@rm -rf .build

