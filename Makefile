DESTDIR?=/usr
PREFIX?=/local

ifneq ($V,1)
Q ?= @
endif

CC	= gcc
CFLAGS	= $(DEBUG) -Wall -Wextra $(INCLUDE) -Winline -pipe 

LDFLAGS	= -L$(DESTDIR)$(PREFIX)/lib
LIBS    = -lpthread -lrt -lm -lcrypt

SRC	=	megaioind.c comm.c thread.c

OBJ	=	$(SRC:.c=.o)

all:		megaioind

megaioind:	$(OBJ)
	$Q echo [Link]
	$Q $(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

.c.o:
	$Q echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

.PHONY:	clean
clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) megaioind *~ core tags *.bak

.PHONY:	install
install: megaioind
	$Q echo "[Install]"
	$Q cp megaioind		$(DESTDIR)$(PREFIX)/bin
	$Q rm -f $(OBJ) megaioind *~ core tags *.bak
   
ifneq ($(WIRINGPI_SUID),0)
	$Q chown root.root	$(DESTDIR)$(PREFIX)/bin/megaioind
	$Q chmod 4755		$(DESTDIR)$(PREFIX)/bin/megaioind
endif
#	$Q mkdir -p		$(DESTDIR)$(PREFIX)/man/man1
#	$Q cp megaio.1		$(DESTDIR)$(PREFIX)/man/man1

.PHONY:	uninstall
uninstall:
	$Q echo "[UnInstall]"
	$Q rm -f $(DESTDIR)$(PREFIX)/bin/megaioind
	$Q rm -f $(DESTDIR)$(PREFIX)/man/man1/megaioind.1
