all: clean
	gcc main.c estruturas_de_dados.c -Wall -o	main

clean:
	rm -f main
