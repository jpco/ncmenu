CC=gcc
CFLAGS=-ggdb

ncmenu: ncmenu.c
	$(CC) $(CFLAGS) -lncurses -o ncmenu ncmenu.c

