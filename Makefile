
 
default:
	gcc -o hit_or_miss hit_or_miss.c
debug: 
	gcc -D DEBUG=1 -o hit_or_miss hit_or_miss.c
clean: 
	mv hit_or_miss log.txt trash

