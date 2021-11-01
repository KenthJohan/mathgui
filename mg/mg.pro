TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


QMAKE_CFLAGS += -Wno-unused-function
QMAKE_CFLAGS += -Wno-missing-braces
QMAKE_CFLAGS += -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-missing-field-initializers

DEFINES += CSC_SRCDIR=\\\"../mg/\\\"
#DEFINES += DEBUG

HEADERS += csc/csc_gft.h
HEADERS += csc/csc_math.h
HEADERS += csc/csc_qf32.h
HEADERS += csc/csc_mf32.h
HEADERS += csc/csc_vf32.h
HEADERS += csc/csc_vf32_misc.h
HEADERS += csc/csc_vu32.h
HEADERS += csc/csc_f32.h
HEADERS += csc/csc_vf32_print.h
HEADERS += csc/csc_vf32_convolution.h
HEADERS += csc/csc_v3f32.h
HEADERS += csc/csc_v4f32.h
HEADERS += csc/csc_m3f32.h
HEADERS += csc/csc_m4f32.h
HEADERS += csc/csc_basic.h
HEADERS += csc/csc_gcam.h
HEADERS += systems.h
HEADERS += system_lines.h
HEADERS += system_opengl.h
HEADERS += system_texture.h
HEADERS += system_mesh.h
HEADERS += system_pointcloud.h
HEADERS += system_camera.h
HEADERS += system_text.h
HEADERS += mg_comp.h
HEADERS += mg_compdef.h
HEADERS += mg_attr.h
HEADERS += mg_send.h
HEADERS += eavnet.h
HEADERS += eavnet_recv.h



SOURCES += mg.c
SOURCES += ../flecs/flecs.c

INCLUDEPATH += C:/msys64/mingw64/include
INCLUDEPATH += C:/msys64/mingw64/include/freetype2
INCLUDEPATH += ../flecs



DEFINES += NNG_STATIC_LIB
DEFINES += GLEW_STATIC

#https://github.com/harfbuzz/harfbuzz/issues/720






LIBS += -Wl,-Bstatic -Wl,--start-group \
-lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--no-undefined -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid \
-static-libgcc \
-lglew32 -lopengl32 \
-mms-bitfields -lfreetype -lbz2 -lz -lpng16 -lz -lharfbuzz -lfreetype -lm -lusp10 -lgdi32 -lrpcrt4 -ldwrite -lgraphite2 -lstdc++ -lglib-2.0 -lintl -lws2_32 -lole32 -lwinmm -lshlwapi -pthread -lm -lpcre -lbrotlidec -lbrotlicommon \
-lnng \
-lz \
-lws2_32 -lmswsock -ladvapi32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32
LIBS += -Wl,--end-group
