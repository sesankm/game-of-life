CFLAGS=-Wall `pkg-config --cflags sdl2`
LIBS=`pkg-config --libs sdl2`

gol: 
	$(CC) $(CFLAGS) -o GOL gol.c $(LIBS)
