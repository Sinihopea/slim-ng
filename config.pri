# config.pri

PROJECT_ROOT = $$PWD

# Slim is not a Qt project
QT =
CONFIG -= qt

# Compilers
# QMAKE_CC = clang
# QMAKE_CXX = clang++

# Common compiler flags
QMAKE_CFLAGS += -pedantic

QMAKE_CFLAGS += -Wall
QMAKE_CFLAGS += -Wconversion
QMAKE_CFLAGS += -Werror=format-security
QMAKE_CFLAGS += -Werror=implicit
QMAKE_CFLAGS += -Werror=incompatible-pointer-types
QMAKE_CFLAGS += -Werror=int-conversion
QMAKE_CFLAGS += -Wformat
QMAKE_CFLAGS += -Wformat=2
QMAKE_CFLAGS += -Wimplicit-fallthrough
QMAKE_CFLAGS += -Wpointer-arith

QMAKE_CFLAGS += -fPIE
QMAKE_CFLAGS += -fpermissive
QMAKE_CFLAGS += -fstack-clash-protection
QMAKE_CFLAGS += -fstack-protector-strong
QMAKE_CFLAGS += -fstrict-flex-arrays=3

QMAKE_CFLAGS += -D_GLIBCXX_ASSERTIONS

QMAKE_CXXFLAGS += -pedantic

QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Wcast-align
QMAKE_CXXFLAGS += -Wconversion
QMAKE_CXXFLAGS += -Weffc++
QMAKE_CXXFLAGS += -Werror=format-security
QMAKE_CXXFLAGS += -Werror=implicit
QMAKE_CXXFLAGS += -Werror=incompatible-pointer-types
QMAKE_CXXFLAGS += -Werror=int-conversion
QMAKE_CXXFLAGS += -Wformat
QMAKE_CXXFLAGS += -Wformat=2
QMAKE_CXXFLAGS += -Wimplicit-fallthrough
QMAKE_CXXFLAGS += -Wmisleading-indentation
QMAKE_CXXFLAGS += -Wnon-virtual-dtor
QMAKE_CXXFLAGS += -Wold-style-cast
QMAKE_CXXFLAGS += -Woverloaded-virtual
QMAKE_CXXFLAGS += -Wpointer-arith
QMAKE_CXXFLAGS += -Wshadow
QMAKE_CXXFLAGS += -Wsign-conversion
QMAKE_CXXFLAGS += -Wunused

QMAKE_CXXFLAGS += -fPIE
QMAKE_CXXFLAGS += -fpermissive
QMAKE_CXXFLAGS += -fstack-clash-protection
QMAKE_CXXFLAGS += -fstack-protector-strong
QMAKE_CXXFLAGS += -fstrict-flex-arrays=3

QMAKE_CXXFLAGS += -D_GLIBCXX_ASSERTIONS

# Compiler flags for debug builds only
QMAKE_CFLAGS_DEBUG += -Walloca
QMAKE_CFLAGS_DEBUG += -Wextra
QMAKE_CFLAGS_DEBUG += -Wformat-security
QMAKE_CFLAGS_DEBUG += -Wwrite-strings

QMAKE_CFLAGS_DEBUG += -fstack-usage

QMAKE_CXXFLAGS_DEBUG += -Walloca
QMAKE_CXXFLAGS_DEBUG += -Wextra
QMAKE_CXXFLAGS_DEBUG += -Wformat-security
QMAKE_CXXFLAGS_DEBUG += -Wwrite-strings

QMAKE_CXXFLAGS_DEBUG += -fstack-usage

# Compiler flags for release builds only
QMAKE_CFLAGS_RELEASE += -D_FORTIFY_SOURCE=3
QMAKE_CFLAGS_RELEASE += -U_FORTIFY_SOURCE

QMAKE_CXXFLAGS_RELEASE += -D_FORTIFY_SOURCE=3
QMAKE_CXXFLAGS_RELEASE += -U_FORTIFY_SOURCE

# Linker
# QMAKE_LINK = clang++

# Linker flags
QMAKE_LFLAGS += -Wl,--as-needed
QMAKE_LFLAGS += -Wl,--no-copy-dt-needed-entries
QMAKE_LFLAGS += -Wl,-z,nodlopen
QMAKE_LFLAGS += -Wl,-z,noexecstack
QMAKE_LFLAGS += -Wl,-z,now
QMAKE_LFLAGS += -Wl,-z,relro
QMAKE_LFLAGS += -Wl,-z,separate-code
QMAKE_LFLAGS += -pie

# Build configuration
CONFIG += debug_and_release
CONFIG += debug_and_release_target

isEmpty(PREFIX) {
	PREFIX = /usr
}

isEmpty(SYSCONFDIR) {
	SYSCONFDIR = /etc
}

isEmpty(BINDIR) {
	BINDIR = $${PREFIX}/bin
}

isEmpty(LIBDIR) {
	LIBDIR = $${PREFIX}/lib64
}

isEmpty(DATADIR) {
	LIBDIR = $${PREFIX}/share
}

isEmpty(MANDIR) {
	MANDIR = $${PREFIX}/man
}

isEmpty(INCLUDEDIR) {
	INCLUDEDIR = $${PREFIX}/include
}

