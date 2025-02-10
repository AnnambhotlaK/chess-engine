all:
	gcc -oFast main.c -o main
	x86_64-w64-mingw32-gcc -oFast main.c -o main.exe

debug:
	gcc -oFast main.c -o main
	x86_64-w64-mingw32-gcc main.c -o main.exe
