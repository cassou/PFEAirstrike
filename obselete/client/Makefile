
CC = gcc
INCLUDES = -I./includes
LIBS = $(shell find ./libs/ -name '*.a')
CFILES:= $(shell find ./src/ -name '*.c')
OBJECTS:= $(CFILES:.c=.o)
CFLAGS = -g -Wall `sdl-config --libs` -lSDL_image -lSDL_mixer  $(INCLUDES)


all:  client

client: $(OBJECTS)
	$(CC) -o $@ $(CFLAGS) $(OBJECTS) $(LIBS) 


	
clean:
	rm -f `find ./src/ -name '*.[od]' -or -name '*~' -or -name '*.bb' -or -name '*.bbg'`
	#rm ./src/client