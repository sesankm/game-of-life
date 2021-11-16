PKGS=glfw3 flew
CFLAGS=-Wall -Wextra -std=c11 `pkg-config --cflags sdl2`
LIBS=`pkg-config --libs sdl2` -lm

gol: 
	$(CC) $(CFLAGS) -o GOL gol.c $(LIBS)
