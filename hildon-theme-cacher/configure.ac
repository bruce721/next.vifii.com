AC_INIT(Makefile.am)
AM_INIT_AUTOMAKE(hildon-theme-cacher, 0.4.3)
AM_CONFIG_HEADER(config.h)

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_INSTALL
AC_HEADER_STDC

CFLAGS="$CFLAGS -Wall -pedantic -Wmissing-prototypes -Wmissing-declarations"

GTK_VERSION=2.6.4

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= $GTK_VERSION)
AC_SUBST(GTK_LIBS)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_VERSION)

AC_OUTPUT([Makefile                             \
           scripts/Makefile                     \
	   src/Makefile                         \
           scripts/hildon-theme-recache-all])

