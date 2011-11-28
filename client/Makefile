

CC = gcc
INCLUDES =-I ./includes
LIBS = $(shell find ./libs/ -name '*.a')
CFILES:= $(shell find ./src/ -name '*.c')
OBJECTS:= $(CFILES:.c=.o)
CFLAGS = -g -Wall -lSDL_image $(INCLUDES)


all:  client

client: $(OBJECTS)
	$(CC) -o $@ $(CFLAGS) $(OBJECTS) $(LIBS)
	ln -s ./src/client ./client -f

	
clean:
	rm -f `find ./src/ -name '*.[od]' -or -name '*~' -or -name '*.bb' -or -name '*.bbg'`
	rm ./src/client