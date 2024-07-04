Various Windows versions support OpenGL natively, but graphics card manufacturers often replace the native Windows OpenGL libraries with their own libraries that support specific features of their hardware.
Since most Windows environments do not contain a native compilation suite, installing the necessary header files and libraries differs depending on the compiler used.

Your simplest option to get a full toolchain on Windows is to use the MSYS2 platform, which provides an easy to use environment for installing and running native Windows software. MSYS2 uses the MinGW toolchain and the pacman package manager to easily install various packages.

Start by downloading and installing MSYS2 from here. Just use the default locations. Only do the basic MSYS2 installation. Do not do any of the pacman steps on the MSYS2 install page.

Once installed launch MSYS and update the packages to the latests version using.

pacman -Syu
This may close the window in some cases. Relaunch the window and rerun the command until it says everything is up to date.
Now install the compiler toolchain using

pacman -S --needed base-devel mingw-w64-x86_64-toolchain
Just do all which installs way more than we need but that's OK.
Now launch MSYS MinGW 64-bit from the Start window. 

GLUT
Now install GLUT and GLEW. It is as simple as doing
pacman -S mingw-w64-x86_64-freeglut
pacman -S mingw-w64-x86_64-glew
You can install any other package you want exacly the same way. To compile a program using GLUT and GLEW do
gcc -Wall -DUSEGLEW -o foo foo.c -lfreeglut -lglew32 -lglu32 -lopengl32 -lm
Note that in my example programs, using GLEW is required on some systems and not on others. So I use the compiler flag -DUSEGLEW to conditionally compile in GLEW when needed. My system does not require or support GLEW, so if you do not make using GLEW conditional the build will fail on my hardware.

After, navigate to your MSYS MinGW directory and launch using
make clean
./project
