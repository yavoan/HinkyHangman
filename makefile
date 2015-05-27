CFALGS = -g
CC = g++

all: hinky

clean: 
	rm hinky
run:
	./hinky wordlist.txt
hinky:
	$(CC) -o hinky hinky.cpp

