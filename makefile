all:
	gcc -oFast main.c -o main
	x86_64-w64-mingw32-gcc -oFast main.c -o main.exe

debug:
	gcc -oFast bbc.c -o bbc
	x86_64-w64-mingw32-gcc bbc.c -o bbc.exe
