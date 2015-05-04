DIR_SRC = src
DIR_OBJ = obj
DIR_BIN = bin

VPATH = $(DIR_SRC)/
FLAGS = -Wall -pedantic

soobnetter:	main.o splash.o address.o network.o prompt.o io.o
	gcc $(FLAGS) -o $(DIR_BIN)/soobneter $(DIR_OBJ)/* -lm

main.o:	main.c splash.h network.h io.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/main.o $(DIR_SRC)/main.c

splash.o:	splash.c splash.h appinfo.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/splash.o $(DIR_SRC)/splash.c

address.o:	address.c address.h consts.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/address.o $(DIR_SRC)/address.c

network.o:	network.c network.h consts.h address.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/network.o $(DIR_SRC)/network.c

prompt.o:	prompt.c prompt.h address.h network.h consts.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/prompt.o $(DIR_SRC)/prompt.c

io.o:	io.c io.h network.h prompt.h
	gcc $(FLAGS) -c -o $(DIR_OBJ)/io.o $(DIR_SRC)/io.c

clean:
	rm -f $(DIR_OBJ)/*.o $(DIR_BIN)/soobnetter
