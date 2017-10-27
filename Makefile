main: main.c
	gcc -O3 main.c -lmpc -lmpfr -lgmp -o main

clean:
	rm -f mandelbrot.* main
