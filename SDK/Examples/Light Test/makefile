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

INCLUDE = -I "../../Includes/Apple"   \
          -I "../../Includes/Quesa"   \
          -I "../Qut"                 \
          -I "../Qut/Unix"            \
          -I "/usr/lib/glib/include/" \

CFLAGS = $(WARN) $(INCLUDE) -c -DQUESA_OS_UNIX=1

LDFLAGS = -L../../Libraries/Unix/Debug/ \
          -L/usr/X11/lib                \
          -L/usr/local/lib              \
          -lquesa -lc -lm -lX11 -lGL -lGLU -lglib -lgtk -lgdk



# Tools
CC = gcc



# Targets
all: obj obj/LightTest.o obj/Qut.o obj/QutUnix.o lighttest

clean:
	rm -fr obj lighttest

obj:
	mkdir obj

obj/LightTest.o: Light\ Test.c
	$(CC) $(CFLAGS) "$^" -o $@

obj/Qut.o: ../Qut/Qut.c
	$(CC) $(CFLAGS) "$^" -o $@

obj/QutUnix.o: ../Qut/Unix/QutUnix.c
	$(CC) $(CFLAGS) "$^" -o $@

lighttest: obj/LightTest.o obj/Qut.o obj/QutUnix.o 
	${CC} obj/LightTest.o obj/Qut.o obj/QutUnix.o -o lighttest ${LDFLAGS}
