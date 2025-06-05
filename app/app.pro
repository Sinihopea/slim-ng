# slim-ng.pro

TEMPLATE = app

include(../config.pri)

CONFIG += debug_and_release

QT =

TARGET = slim

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