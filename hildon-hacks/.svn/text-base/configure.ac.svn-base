#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(hildon-hacks, 0.1)

# Checks for programs.

AC_PROG_INSTALL

### Look for msgmft, this can be done better ###
AC_DEFUN([AC_PROG_MSGFMT],[
AC_CHECK_PROGS(msgfmt,[msgfmt],no)
export msgfmt;
if test $msgfmt = "no" ;
then
        AC_MSG_ERROR([Unable to find the msgfmt application]);
fi
AC_SUBST(msgfmt)
])

AC_PROG_MSGFMT

AC_OUTPUT([
        Makefile
        po/Makefile
        scripts/Makefile
        scripts/hildon-session.sh
])
