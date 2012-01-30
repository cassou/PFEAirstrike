
all: showanim hfinter heightfield

showanim: showanim.c
	$(CC) -Wall -o $@ $^ `sdl-config --cflags --libs` -lSDL_image

hfinter: hfinter.c
	$(CC) -Wall -o $@ $^ `sdl-config --cflags --libs` -lSDL_image

heightfield: heightfield.c
	$(CC) -Wall -g -o $@ $^ `sdl-config --cflags --libs` -lSDL_image

clean:
	rm -f showanim *~ \#*