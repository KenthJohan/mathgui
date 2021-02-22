# Windows Build

* Download https://www.msys2.org/ and follow instructions
* Run `pacman -S mingw-w64-x86_64-toolchain`<br>
https://packages.msys2.org/group/mingw-w64-x86_64-toolchain
* Run `pacman -S mingw64/mingw-w64-x86_64-SDL2`<br>
Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D.
* Run `pacman -S mingw64/mingw-w64-x86_64-glew`<br>
The OpenGL Extension Wrangler Library (GLEW) is a cross-platform open-source C/C++ extension loading library. GLEW provides efficient run-time mechanisms for determining which OpenGL extensions are supported on the target platform. OpenGL core and extension functionality is exposed in a single header file. GLEW has been tested on a variety of operating systems, including Windows, Linux, Mac OS X, FreeBSD, Irix, and Solaris. 
* Run `pacman -S mingw64/mingw-w64-x86_64-clang` qtcreator depends on `clangbackend.exe` and `libclang.dll`
* Run `pacman -S mingw-w64-x86_64-qt-creator`
* Run `pacman -S mingw64/mingw-w64-x86_64-gtk4`
* Run `qtcreator nameofproject.pro`


## Find flags
```
pkg-config --static --cflags --libs sdl2`
```



# Linux Build

* TODO