gcc -xc mg.c ../flecs/flecs.c -omg.exe \
-DCSC_SRCDIR="" \
-DNNG_STATIC_LIB -DGLEW_STATIC \
-I../flecs \
-I../flecs/flecs-os_api-posix/include \
-IC:/msys64/mingw64/include \
-IC:/msys64/mingw64/include/freetype2 \
-LC:/msys64/mingw64/lib \
-Wl,-Bstatic -Wl,--start-group \
-lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--no-undefined -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid \
-static-libgcc \
-lglew32 -lopengl32 \
-mms-bitfields -lfreetype -lbz2 -lz -lpng16 -lz -lharfbuzz -lfreetype -lm -lusp10 -lgdi32 -lrpcrt4 -ldwrite -lgraphite2 -lstdc++ -lglib-2.0 -lintl -lws2_32 -lole32 -lwinmm -lshlwapi -pthread -lm -lpcre -lbrotlidec -lbrotlicommon \
-lnng \
-lz \
-lws2_32 -lmswsock -ladvapi32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 \
-Wl,--end-group

ldd mg.exe > mg_deps.txt