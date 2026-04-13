# slim-ng.pro

TEMPLATE = app
TARGET = slim

VERSION = 0.3.6
RELEASE = 1

LANGUAGE = C++

include(../config.pri)

CONFIG += c17
CONFIG += c++17
# CONFIG += x11

CONFIG(debug, debug|release) {
	message("debug")
	CONFIG += warn_on
	CONFIG += rtti

	OBJECTS_DIR = $$PROJECT_ROOT/build/slim/debug/
} else {
	message("release")
	CONFIG += warn_off
	CONFIG += rtti_off

	OBJECTS_DIR = $$PROJECT_ROOT/build/slim/release/
}

DEFINES += APPNAME=\\\"slim\\\"
DEFINES += VERSION=\\\"1.3.6\\\"
DEFINES += SYSCONFDIR=\\\"/etc\\\"
DEFINES += PKGDATADIR=\\\"/usr/share/slim\\\"
DEFINES += USE_PAM

unix
{
    CONFIG += link_pkgconfig
    PKGCONFIG += x11
    PKGCONFIG += xmu
    PKGCONFIG += xext
    PKGCONFIG += xft
    PKGCONFIG += fontconfig
    PKGCONFIG += xrandr
    PKGCONFIG += libpng
    PKGCONFIG += libjpeg
    PKGCONFIG += pam
    PKGCONFIG += dbus-1
    PKGCONFIG += libcrypt
}

HEADERS += src/app.hpp \
           src/cfg.hpp \
           src/const.hpp \
           src/image.hpp \
           src/log.hpp \
           src/numlock.hpp \
           src/pam.hpp \
           src/panel.hpp \
           src/switchuser.hpp \
           src/util.hpp

# IF CONSOLEKIT, THEN THIS TOO
# src/ck.hpp \

SOURCES += src/main.cpp \
           src/app.cpp \
           src/cfg.cpp \
           src/image.cpp \
           src/jpeg.c \
           src/log.cpp \
           src/numlock.cpp \
           src/pam.cpp \
           src/panel.cpp \
           src/png.c \
           src/switchuser.cpp \
           src/util.cpp

# IF SLIMLOCK, THEN
# src/slimlock.cpp \

# IF CONSOLEKIT, THEN
# src/ck.cpp \

# IF CHECKS: PASS, COPY TO BUILDROOT
# target.path += $${BINDIR}/
# INSTALLS += target

# TODO Needs to copy resources as well
