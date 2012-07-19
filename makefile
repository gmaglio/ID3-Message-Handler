GLIB_CFLAGS:=`pkg-config --cflags glib-2.0`
GLIB_LDFLAGS:=`pkg-config --libs glib-2.0`
CC=gcc
PWD=`pwd`
OBJ=$(PWD)/obj
BIN=$(PWD)/bin
SRC=$(PWD)/src

all: id3_getshm id3_rmshm id3_putshm id3_mcast_sender id3_mcast_receiver

id3_putshm.o: 
	gcc -g -c $(SRC)/id3_putshm.c $(GLIB_CFLAGS) -o $(OBJ)/id3_putshm.o

id3_file.o: 
	gcc -g -c $(SRC)/id3_file.c $(GLIB_CFLAGS) -o $(OBJ)/id3_file.o

id3_getshm.o: 
	gcc -g -c $(SRC)/id3_getshm.c $(GLIB_CFLAGS) -o $(OBJ)/id3_getshm.o

id3_mcast_sender.o: 
	gcc -g -c $(SRC)/id3_mcast_sender.c $(GLIB_CFLAGS) -o $(OBJ)/id3_mcast_sender.o

id3_mcast_receiver.o: 
	gcc -g -c $(SRC)/id3_mcast_receiver.c $(GLIB_CFLAGS) -o $(OBJ)/id3_mcast_receiver.o

id3_data.o: 
	gcc -g -c $(SRC)/id3_data.c $(GLIB_CFLAGS) -o $(OBJ)/id3_data.o

id3_rmshm: 
	gcc -g $(SRC)/id3_rmshm.c $(GLIB_CFLAGS) $(GLIB_LDFLAGS) -o bin/id3_rmshm

id3_putshm: id3_putshm.o id3_file.o id3_data.o 
	gcc $(OBJ)/id3_putshm.o $(OBJ)/id3_file.o $(OBJ)/id3_data.o $(GLIB_LDFLAGS) -o $(BIN)/id3_putshm

id3_getshm: id3_getshm.o id3_file.o id3_data.o
	gcc $(OBJ)/id3_getshm.o $(OBJ)/id3_data.o $(GLIB_LDFLAGS) -o $(BIN)/id3_getshm

id3_mcast_sender: id3_mcast_sender.o 
	gcc $(OBJ)/id3_mcast_sender.o $(GLIB_LDFLAGS) -o $(BIN)/id3_mcast_sender

id3_mcast_receiver: id3_mcast_receiver.o 
	gcc $(OBJ)/id3_mcast_receiver.o $(GLIB_LDFLAGS) -o $(BIN)/id3_mcast_receiver


clean:
	/bin/rm -f $(BIN)/* $(OBJ)/*.o
