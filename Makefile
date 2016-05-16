SDLPATH = src/SDL

 
CFLAGS = -W -Wall -Wextra -I$(SDLPATH)/include
 
LDFLAGS = -L  -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf 
# $(SDLPATH)/lib -lmingw32

SRC= $(wildcard src/*.c)
OBJ= $(SRC:.c=.o)

CC = gcc
PROG = bin/exec

all: mgl

mgl: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)