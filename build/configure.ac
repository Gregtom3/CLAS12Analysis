AC_INIT(clas12ana,[1.00])
AC_CONFIG_SRCDIR([configure.ac])
AM_INIT_AUTOMAKE

AC_PROG_CXX(g++)
LT_INIT([disable-static])

dnl   no point in suppressing warnings people should
dnl   at least see them, so here we go for g++: -Wall
dnl   make warnings fatal errors: -Werror
if test $ac_cv_prog_gxx = yes; then
   CXXFLAGS="$CXXFLAGS -Wall -Werror"
fi

dnl test for root 6
if test `root-config --version | gawk '{print $1>=6.?"1":"0"}'` = 1; then
CINTDEFS=" -noIncludePaths  -inlineInputHeader "
AC_SUBST(CINTDEFS)
fi
AM_CONDITIONAL([MAKEROOT6],[test `root-config --version | gawk '{print $1>=6.?"1":"0"}'` = 1])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
