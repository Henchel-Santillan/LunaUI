# LunaUI

LunaUI is a Qt6 Widgets-based desktop application that accompanies LunaSys, both of which are part of the Luna project. As a desktop user interface, the purpose of LunaUI is to provide , and to function as the application-level component fitted into a functional embedded system.

## Build Instructions

At this time, only build instructions for Windows is available.

### Prerequisites
LunaUI is developed using `Qt 6.4.1`, is built using `CMake` and `Ninja`, and is compiled using `MinGW 11.20` for 64-bit systems. Qt6 requires a minimum CMake version of `3.16`. The easiest way to satisfy these dependency requirements is to simply download the Qt for open-source development for free and check off MinGW, CMake, and Ninja using the Online Installer. On Windows, add g++ (from MinGW), CMake, and Ninja to the system PATH. Add the path to the Qt binaries (`<QT_INSTALL_DIR>\<QT_VERSION>\<MINGW_ARCH>\bin`) to the system PATH as well.

### Build (Windows)

These steps are applicable _only_ once the prerequisites have been met.

1. Clone the project using HTTPS.

`git clone https://github.com/Henchel-Santillan/LunaUI.git`

2. Go into the root of the cloned repository, create a build directory, and change into the build directory.
```
cd <path\to\LunaUI\clone>
mkdir build && cd build
```

3. Once in the build directory, run the `cmake` command with the following flags:

```
cmake -DCMAKE_PREFIX_PATH=<QT_INSTALL_DIR>\<QT_VERSION>\<MINGW_ARCH>\lib\cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_CXX_COMPILER:FILEPATH=<QT_INSTALL_DIR>\Tools\<MINGW_VERSION_ARCH>\bin\g++.exe -DCMAKE_GENERATOR:STRING=Ninja -DCMAKE_MAKE_PROGRAM:FILEPATH=<QT_INSTALL_DIR>\Tools\Ninja\ninja.exe -S <path\to\LunaUI\clone\root> -B <path\to\LunaUI\clone\build\dir>
```

* `<QT_INSTALL_DIR>` is the install path you specified using the online installer (e.g. `C:\Qt\` on Windows)
* `<QT_VERSION>` is the full Qt version of your installation, e.g. `6.4.1`
* `<MINGW_VERSION_ARCH>` is MinGW bundled with the Qt installation, specifying both version and architecture (e.g. `mingw1120_6`)

4. Run the `ninja` command.
5. Run `start LunaUI.exe`.
