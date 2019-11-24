CC = gcc
RM = rm -f
ST = `which strip`


NAME = toscosh


INCLUDE = -I.
LIBS = 
CFLAGS = $(INCLUDE)

# files
OBJECTS = main.o     \
		  lib.o  \
		  exec.o \
		  event.o \
		  term.o

SOURCES = main.c     \
		  lib.c  \
		  exec.c \
		  event.c \
		  term.c 


# rules
.SUFFIXES: .c
.c.o:
		$(CC) $(CFLAGS) -c -o $@ $<

# build
all:	$(OBJECTS)
		$(CC) -o $(NAME) $(OBJECTS) $(LIBS)
		$(ST) $(NAME)

clean:
		$(RM) $(NAME) $(OBJECTS)
		$(RM) *~