main:
	gcc -pthread -g main.c -o main -lm
mem:
	gcc -g mem.c -o mem -lm
all:
	gcc -pthread -g main.c -o main -lm	
	gcc -g mem.c -o mem -lm
clean:
	rm -f main
	rm -f mem
