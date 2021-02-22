TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += CSC_SRCDIR=\\\"../demo2/\\\"
#DEFINES += DEBUG
INCLUDEPATH += ../flecs
INCLUDEPATH += ../flecs/flecs-os_api-posix/include

QMAKE_CFLAGS += -Wno-unused-function

SOURCES += sender.c
SOURCES += ../flecs/flecs.c

LIBS += -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglew32 -lSDL2_net

LIBS += -lws2_32 -lwsock32 -lpthread
