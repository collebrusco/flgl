# Building FLGL Based GFX Projects
flgl uses [CMake](https://cmake.org/) as its included build system. Typical flgl projects will then use CMake as well.      
This file will walk you through the process of building a typical project that uses flgl for graphics. 
The process is pretty much the same as for any CMake project but I'll still briefly go through it here.
      
[Install CMake](https://cmake.org/download/) and make sure the `cmake` command is on your path.     
CMake works by reading generalized cross platform build instructions in `CMakeLists.txt` and generating a platform specific build system for the project.      

## MacOS / Linux
Run `cmake .` in the project repo directory to get CMake to generate your Makefile, then run `make` to build your executable. Run with ./<exec_name>     

## Windows
On windows, you must tell CMake what build system to use.        
Because I like using a bash command line, my personally prefered method is to install the [winlibs MinGW clang compiler](https://winlibs.com/), [Git Bash](https://gitforwindows.org/), and [Make for Windows](https://gnuwin32.sourceforge.net/packages/make.htm).         
Add the minGW and make executables to your path and then with Git Bash, run 
`cmake -G "MinGW Makefiles" .` followed by `make` and build your exe. Don't move the exe; create a shortcut if you'd like.       
      
CMake is also capable of working with Visual Studio and other Windows build systems.

      
