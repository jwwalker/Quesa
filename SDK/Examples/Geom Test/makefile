#***************************************************************************
# Makefile for Unix builds
# dair@webthing.net
#
# NB - Real makefile hackers required: should we be using autoconf for this?
#***************************************************************************



# Flags
WARN = -Wall -Wpointer-arith -Wcast-qual -Waggregate-return            \
       -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
       -Wredundant-decls -Wno-multichar -Wno-conversion

INCLUDE = -include "../../../Development/Source/Platform/Unix/E3UnixPrefix.h" \
          -I "../../Includes/Apple"   \
          -I "../../Includes/Quesa"   \
          -I "../Qut"                 \
          -I "../Qut/Unix"            \
          -I "/usr/lib/glib/include/" \

CFLAGS = $(WARN) $(INCLUDE) -c

LDFLAGS = -L../../Libraries/Unix/Debug/ \
          -L/usr/X11/lib                \
          -L/usr/local/lib              \
          -lquesa -lc -lm -lX11 -lGL -lGLU -lglib -lgtk -lgdk



# Tools
CC = gcc



# Targets
all: obj obj/GeomTest.o obj/Qut.o obj/QutUnix.o geomtest

clean:
	rm -fr obj geomtest

obj:
	mkdir obj

obj/GeomTest.o: Geom\ Test.c
	$(CC) $(CFLAGS) "$^" -o $@

obj/Qut.o: ../Qut/Qut.c
	$(CC) $(CFLAGS) "$^" -o $@

obj/QutUnix.o: ../Qut/Unix/QutUnix.c
	$(CC) $(CFLAGS) "$^" -o $@

geomtest: obj/GeomTest.o obj/Qut.o obj/QutUnix.o 
	${CC} obj/GeomTest.o obj/Qut.o obj/QutUnix.o -o geomtest ${LDFLAGS}
