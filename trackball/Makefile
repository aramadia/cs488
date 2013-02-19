# This make file builds the interaction demo program. 
#
# Makefile targets:
#
# all       -> builds the exe
# (nothing) -> builds the exe
# exe       -> builds the exe
# clean     -> removes all the object files and core files in the current dir.
# depend    -> appends dependancy info to this makefile
#


# define what to make (Machine dependent stuff)
BASENAME=intdemo
EXTRALIBS =

# define things needed during the make

CC=gcc


INCLUDES = \
	-I.

CFLAGS= \
        $(INCLUDES) \
	-g $(EXTRAFLAGS)

LIBS= \
	-lGL -lGLU \
	-L/usr/X11R6/lib -lX11 -lXext -lm -lpthread 

EXE      = $(BASENAME)

OBJS = \
        intdemo.o \
        OGLwin.o \
        events.o \
        trackball.o

SRCS = $(OBJS:.o=.c)

# define the rules for building the library, and the shells

.c.o:
	$(CC) $(CFLAGS) -c $< -o $*.o

exe:    $(EXE)

$(EXE):  $(OBJS) 
	$(CC) -o $(EXE) $(CFLAGS) $(OBJS)  $(LIBS) $(EXTRALIBS)

depend:
	makedepend -s "# DO NOT DELETE" -- $(INCLUDES) -- $(SRCS)

clean:
	rm -f *.o core

realclean: clean
	rm -f $(EXE)

all: exe


# DO NOT DELETE




