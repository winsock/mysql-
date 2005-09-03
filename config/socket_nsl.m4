dnl @synopsis LIB_SOCKET_NSL
dnl 
dnl This macro figures out what libraries are required on this platform
dnl to link sockets programs.
dnl
dnl The common cases are not to need any extra libraries, or to need
dnl -lsocket and -lnsl.  We need to avoid linking with libnsl unless
dnl we need it, though, since on some OSes where it isn't necessary it
dnl will totally break networking.  Unisys also includes gethostbyname()
dnl in libsocket but needs libnsl for socket().
dnl
dnl @version 1.3, 2005/08/30
dnl @author Russ Allbery <rra@stanford.edu> and Warren Young <warren@etr-usa.com>

AC_DEFUN([LIB_SOCKET_NSL],
[
	AC_SEARCH_LIBS([gethostbyname], [nsl])
	AC_SEARCH_LIBS([socket], [socket], , [
		AC_CHECK_LIB([nsl], [socket], LIBS="$LIBS -lsocket -lnsl", ,
		-lsocket)])
])

