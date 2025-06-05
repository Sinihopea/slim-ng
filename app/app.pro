# slim-ng.pro

TEMPLATE = app

QT =

TARGET = slim

HEADERS += src/app.hpp \
           src/cfg.hpp \
           src/ck.hpp \
           src/const.h \
           src/image.hpp \
           src/log.hpp \
           src/numlock.hpp \
           src/pam.hpp \
           src/panel.hpp \
           src/switchuser.hpp \
           src/util.hpp

SOURCES += src/app.cpp \
           src/cfg.cpp \
           src/ck.cpp \
           src/image.cpp \
           src/jpeg.c \
           src/log.cpp \
           src/main.cpp \
           src/numlock.cpp \
           src/pam.cpp \
           src/panel.cpp \
           src/png.c \
           src/slimlock.cpp \
           src/switchuser.cpp \
           src/util.cpp
