# Makefile for Mailbox server
PROG=	mailbox
SRCS=	main.c mailbox.c message_queue.c

DPADD+=	${LIBSYS}
LDADD+=	-lsys

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk> 
