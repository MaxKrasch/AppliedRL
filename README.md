HaxBall Group-F
===================================

Empty starting project for ARL 2022.

Some instructions
-----------------

After cloning, you have to add the submodule to get the HaxBall code:

```console
git submodule update --init
```

Compilation should work with the normal procedure:

```console
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j 8
```

Hopefully, it is now possilbe to start the agent in the build directory:

```console
./HaxBallAgentGroup-F
```

Use an IDE such as QtCreator to open the full project. Your IDE should be able to open the CMakeLists.txt directly:

```console
qtcreator CMakeLists.txt
```

Qt and QtCreator, CMake
-----------------------

On Mac and Linux, you can most likely use your paket manager for all three.

For Windows, download from the (Qt Website)[https://www.qt.io/offline-installers] the offline installer with the latest Qt 5.15.x version (matching the EIKON).
Disable during the installation your internet to avoid creating a Qt Account.
During the setup, select also MinGW to get a running C++ Compiler on your computer.
Select the QtCreator checkbox to install the IDE.

CMake is available at (Kitware's Website)[https://cmake.org/download/], just install it.

Now, you can start Qtcreator and open directly the CMakeLists.txt as C++ project.
