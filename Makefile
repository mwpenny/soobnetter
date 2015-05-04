OBJ = main.o splash.o address.o network.o prompt.o io.o
DEBUG = -Wall -pedantic

soobnetter:	$(OBJ)
	gcc $(DEBUG) -o soobneter $(OBJ)

main.o:	main.c splash.h network.h io.h
	gcc $(DEBUG) -c -o main.o main.c

splash.o:	splash.c splash.h appinfo.h
	gcc $(DEBUG) -c -o splash.o splash.c

address.o:	address.c address.h consts.h
	gcc $(DEBUG) -c -o address.o address.c

network.o:	network.c network.h consts.h address.h
	gcc $(DEBUG) -c -o network.o network.c

prompt.o:	prompt.c prompt.h address.h network.h consts.h
	gcc $(DEBUG) -c -o prompt.o prompt.c

io.o:	io.c io.h network.h prompt.h
	gcc $(DEBUG) -c -o io.o io.c

clean:
	rm -f *.o soobnetter