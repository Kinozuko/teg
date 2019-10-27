# Variables

CC = g++
CFLAGS = -g
SRC = main.cpp
EJECUTABLE = main

# Archivo ejecutable

all : $(SRC) $(OBJS)
	$(CC) $(CFLAGS) $(SRC) $(OBJS) -o $(EJECUTABLE)
	
# Lista de objetos

# Eliminacion

clean:
	rm *.o *~
