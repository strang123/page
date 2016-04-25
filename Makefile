

all: 
	gcc -D DEBUG=$(debug) -o hit_or_miss hit_or_miss.c

clean: 
	mv hit_or_miss log.txt trash

