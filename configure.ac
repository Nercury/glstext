AC_INIT([libglstext], [0.0.0], [nercury@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([-Wall -Werror])
AC_PROG_LIBTOOL
AC_PROG_CXX

# Check for standard C++ library
AC_CHECK_LIB(stdc++, main)

# Check for SDL
SDL_VERSION=1.1.8
AM_PATH_SDL($SDL_VERSION, :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))

case "$host" in
        *-*-mingw*|*-*-cygwin*)
                AC_DEFINE(PLATFORM_WIN32, 1, [Platform is Win32])
                SYS_GL_LIBS="-lGL -lGLU"
                ;;
        *-*-linux*)
                AC_DEFINE(PLATFORM_LINUX, 1, [Platform is Linux])
                SYS_GL_LIBS="-lGL -lGLU"
                ;;
        *-*-darwin*)
        AC_MSG_WARN([Hello])
                AC_DEFINE(PLATFORM_APPLE, 1, [Platform is Apple])
                SYS_GL_LIBS="-framework AGL -framework OpenGL"
                ;;
        *)
                AC_MSG_WARN([*** Please add $host to configure.ac checks!])
                ;;
esac
AC_SUBST(SYS_GL_LIBS)

CFLAGS="$CFLAGS $SDL_CFLAGS -lSDL_ttf"
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS -lSDL_ttf"
LIBS="$LIBS $SDL_LIBS $SYS_GL_LIBS"

AC_CONFIG_FILES([Makefile glstext/Makefile test/Makefile])
AC_OUTPUT
