dnl $Id$
dnl config.m4 for extension str

PHP_ARG_ENABLE(str, whether to enable str support,
[  --enable-str           Enable str support])

if test "$PHP_STR" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-str -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/str.h"  # you most likely want to change this
  dnl if test -r $PHP_STR/$SEARCH_FOR; then # path given as parameter
  dnl   STR_DIR=$PHP_STR
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for str files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       STR_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$STR_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the str distribution])
  dnl fi

  dnl # --with-str -> add include path
  dnl PHP_ADD_INCLUDE($STR_DIR/include)

  dnl # --with-str -> check for lib and symbol presence
  dnl LIBNAME=str # you may want to change this
  dnl LIBSYMBOL=str # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $STR_DIR/lib, STR_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_STRLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong str lib version or lib not found])
  dnl ],[
  dnl   -L$STR_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(STR_SHARED_LIBADD)

  PHP_NEW_EXTENSION(str, str.c, $ext_shared)
fi
