

CC = gcc
INCLUDES =-I ./includes
CFILES:= $(shell find ./src/ -name '*.c')
OBJECTS:= $(CFILES:.c=.o)
CFLAGS = -g -Wall -lenet $(INCLUDES)


all:  client

client: $(OBJECTS)
	$(CC) -o $@ $(CFLAGS) $(OBJECTS) 
	ln -s ./src/client ./client -f

	
clean:
	rm -f `find ./src/ -name '*.[od]' -or -name '*~' -or -name '*.bb' -or -name '*.bbg'`
	rm ./src/client