##
##  @(#)Makefile	1.00	Jan. 11, 2000
##
##  Makefile for syslogd/klogd
##  $Id: Makefile 7519 2010-06-25 18:29:09Z arun $

PROG=	syslogd
PIDFILE= -DPIDFILE=\"/var/run/syslogd.pid\"
CPPFLAGS+= -Ddebug=0 ${PIDFILE}
BINMODE= 700
MAN=

.include <bsd.prog.mk>
