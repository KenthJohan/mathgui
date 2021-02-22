TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += CSC_SRCDIR=\\\"../mathgui/\\\"
#DEFINES += DEBUG

HEADERS += csc/csc_math.h
HEADERS += csc/csc_qf32.h
HEADERS += csc/csc_v3f32.h
HEADERS += csc/csc_v4f32.h
HEADERS += components.h
HEADERS += systems.h
HEADERS += eavnet.h


QMAKE_CFLAGS += -Wno-unused-function
QMAKE_CFLAGS += -Wno-missing-braces
QMAKE_CFLAGS += -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-missing-field-initializers

SOURCES += mathgui.c
SOURCES += ../flecs/flecs.c
#SOURCES += flecs/flecs_meta.c
#SOURCES += flecs/flecs_components_http.c
#SOURCES += flecs/flecs_dash.c
#SOURCES += flecs/flecs_monitor.c
#SOURCES += flecs/flecs_rest.c
#SOURCES += flecs/flecs_systems_civetweb.c
#SOURCES += flecs/flecs_player.c
#SOURCES += flecs/flecs_json.c


INCLUDEPATH += ../flecs
INCLUDEPATH += ../flecs/flecs-os_api-posix/include


#LIBS += -lnng
#LIBS += -lws2_32 -lwsock32 -lpthread

LIBS += -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglew32


DEFINES += NNG_STATIC_LIB
LIBS += -Wl,-Bstatic
LIBS += -lnng
LIBS += -lws2_32 -lmswsock -ladvapi32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32
