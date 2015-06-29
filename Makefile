DIR_SRC = src
DIR_OBJ = obj
DIR_BIN = bin

VPATH = $(DIR_SRC)/
FLAGS = -Wall -pedantic

all: dirs soobnetter

dirs:
	mkdir -p $(DIR_OBJ)/ $(DIR_BIN)/

soobnetter:	main.o splash.o address.o network.o prompt.o io.o args.o list.o parser.o
	gcc $(FLAGS) -o $(DIR_BIN)/soobnetter $(DIR_OBJ)/* -lm

main.o:	main.c args.h list.h splash.h network.h io.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/main.o $(DIR_SRC)/main.c

splash.o:	splash.c splash.h appinfo.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/splash.o $(DIR_SRC)/splash.c

address.o:	address.c address.h consts.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/address.o $(DIR_SRC)/address.c

network.o:	network.c network.h consts.h address.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/network.o $(DIR_SRC)/network.c

prompt.o:	prompt.c prompt.h parser.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/prompt.o $(DIR_SRC)/prompt.c

io.o:	io.c io.h network.h prompt.h parser.h args.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/io.o $(DIR_SRC)/io.c

args.o:	args.c args.h list.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/args.o $(DIR_SRC)/args.c

list.o:	list.c list.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/list.o $(DIR_SRC)/list.c

parser.o:	parser.c parser.h consts.h address.h network.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/parser.o $(DIR_SRC)/parser.c

clean:
	rm -rf $(DIR_OBJ)/ $(DIR_BIN)/
