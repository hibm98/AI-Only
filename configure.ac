#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([aionlyd], [0.2], [hibm98@gmail.com])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror])
PKG_PROG_PKG_CONFIG

AC_CONFIG_SRCDIR([src/daemon.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Remember externally set CFLAGS
EXTERNAL_CFLAGS="$CFLAGS"

# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_CC_C_O
AM_PROG_AR

# Reset the externally set CFLAGS after calling AC_PROG*
CFLAGS="$EXTERNAL_CFLAGS"
AX_CXX_COMPILE_STDCXX_11

# Use the C++ Language and compiler for the following checks
AC_LANG([C++])

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_self], [], AC_MSG_ERROR([Not found pthread library. Please install it.]))
AC_CHECK_LIB([lua5.2], [lua_close], [], AC_MSG_ERROR([Not found lua 5.2 library. Please install it.])) 

# Checks for header files.
AC_CHECK_HEADERS([syslog.h unistd.h pthread.h lua5.2/lua.h lua5.2/lauxlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_FORK

AX_CXX_COMPILE_STDCXX_11

# Check for argument.
AC_ARG_ENABLE(debug,
	AS_HELP_STRING(
		[--enable-debug],
		[enable debugging, default: no]),
		[case "${enableval}" in
		  yes)	debug=true ;;
		  no)	debug=false ;;
		  *)	AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
		 esac],
		[debug=false])
AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")
AM_COND_IF(DEBUG,
	AC_DEFINE(DEBUG, 1, [Define to 0 if this is a release build]),
	AC_DEFINE(DEBUG, 0, [Define to 1 or higher if this is a debug build]))

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
