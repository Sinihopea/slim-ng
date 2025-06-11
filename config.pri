# config.pri

PROJECT_ROOT = $$PWD

# QMAKE_CC = clang
QMAKE_CFLAGS += -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough -Werror=format-security
QMAKE_CFLAGS += -D_GLIBCXX_ASSERTIONS
QMAKE_CFLAGS += -fstrict-flex-arrays=3 -fstack-clash-protection -fstack-protector-strong
# QMAKE_CFLAGS += -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--as-needed -Wl,--no-copy-dt-needed-entries
QMAKE_CFLAGS += -Werror=implicit -Werror=incompatible-pointer-types -Werror=int-conversion
QMAKE_CFLAGS_DEBUG += -O0
QMAKE_CFLAGS_RELEASE += -O2 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3

# QMAKE_CXX = clang++
QMAKE_CXXFLAGS +=  -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough -Werror=format-security
QMAKE_CXXFLAGS += -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS
QMAKE_CXXFLAGS += -fstrict-flex-arrays=3 -fstack-clash-protection -fstack-protector-strong
# QMAKE_CXXFLAGS += -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--as-needed -Wl,--no-copy-dt-needed-entries
QMAKE_CXXFLAGS += -Werror=implicit -Werror=incompatible-pointer-types -Werror=int-conversion
QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_RELEASE += -O2

# QMAKE_LFLAGS += -Wl,-z,nodlopen -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--as-needed -Wl,--no-copy-dt-needed-entries

CONFIG += debug_and_release

CONFIG(debug, debug|release) {
	message("debug")
	DESTDIR = debug
	OBJECTS_DIR = $$PROJECT_ROOT/build/slim/debug/
	MOC_DIR = $$PROJECT_ROOT/build/slim/debug/moc/

} else {
	message("release")
	DESTDIR = release
	OBJECTS_DIR = $$PROJECT_ROOT/build/slim/release/
	MOC_DIR = $$PROJECT_ROOT/build/slim/release/moc/
}

