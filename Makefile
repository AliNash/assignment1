all: functions.h
	gcc Assignment_1.c functions.c -o shell.exe

clear:
	rm -rf *.c *.o
