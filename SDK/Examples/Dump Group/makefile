#***************************************************************************
# Makefile for Unix builds
# dair@webthing.net
#
# NB - Real makefile hackers required: should we be using autoconf for this?
#***************************************************************************



# Flags
WARN = -Wall -Wpointer-arith -Wcast-qual -Waggregate-return            \
       -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
       -Wredundant-decls -Wno-multichar -Wno-conversion                \
       -Wno-unknown-pragmas -Wno-unused

INCLUDE = -I "../../Includes/Apple" \
          -I "../../Includes/Quesa" \
          -I "../Qut"               \
          -I "../Qut/Unix"          \
          -I "/usr/include/gtk-1.2/"  \
          -I "/usr/include/glib-1.2/" \
          -I "/usr/lib/glib/include/" \

CFLAGS = $(WARN) $(INCLUDE) -c -DQUESA_OS_UNIX=1

LDFLAGS = -L../../Libraries/Unix/Debug/ \
          -L/usr/X11R6/lib              \
          -L/usr/local/lib              \
          -lquesa -lc -lm -lX11 -lGL -lGLU -lglib -lgtk -lgdk



# Tools
CC = gcc



# Targets
all: obj obj/DumpGroup.o dumpgroup

clean:
	rm -fr obj dumpgroup

obj:
	mkdir obj

obj/DumpGroup.o: Dump\ Group.c
	$(CC) $(CFLAGS) "$^" -o $@

dumpgroup: obj/DumpGroup.o
	${CC} obj/DumpGroup.o -o dumpgroup ${LDFLAGS}
