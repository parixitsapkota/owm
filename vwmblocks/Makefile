PREFIX  := /usr/local
CC      := gcc
CFLAGS  := -pedantic -Wall -Wno-deprecated-declarations -Os
LDFLAGS := -lX11

# FreeBSD (uncomment)
#LDFLAGS += -L/usr/local/lib -I/usr/local/include
# # OpenBSD (uncomment)
#LDFLAGS += -L/usr/X11R6/lib -I/usr/X11R6/include

all: options vwmblocks

options:
	@echo vwmblocks build options:
	@echo "CFLAGS  = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC      = ${CC}"

vwmblocks: vwmblocks.c vblocks.def.h vblocks.h
	${CC} -o vwmblocks vwmblocks.c ${CFLAGS} ${LDFLAGS}

vblocks.h:
	cp vblocks.def.h $@

clean:
	rm -f *.o *.gch vwmblocks vblocks.h

install: vwmblocks
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f vwmblocks ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/vwmblocks

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/vwmblocks

.PHONY: all options clean install uninstall
