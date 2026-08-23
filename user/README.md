# Building an flgl project

flgl is **vendored and built with plain `make`** — no cmake, and nothing to
install system-wide beyond a C/C++ compiler and `make` (the OS windowing/audio
frameworks flgl needs are always present). You add flgl as a git submodule, run
the setup script once, and build.

## Quick start

From an empty project directory:

```sh
git init
git submodule add https://github.com/collebrusco/flgl.git lib/flgl
git submodule update --init --recursive
./lib/flgl/user/setup.sh
make
```

`setup.sh` drops three things into your project:

- `Makefile`     — a tiny build file that includes `lib/flgl/flgl.mk`
- `.gitignore`   — ignores `build/`
- `src/main.cpp` — an flgl hello-world to start from

Then:

```sh
make                  # build with all flgl modules
make DISABLE=coreal   # build without the audio module
make run              # build and run (./build/untitled)
make clean
```

## How the build works

All the cross-platform knowledge — platform detection, which modules exist,
compiler/linker flags — lives in `lib/flgl/flgl.mk`. Your `Makefile` just
includes it and uses the handles it exports (`FLGL_CPPFLAGS`, `FLGL_LDLIBS`,
`FLGL_LIB`). Building your app also (re)builds `lib/flgl/build/libflgl.a` with
the matching module set.

To permanently disable a module, set it above the include in your `Makefile`:

```make
FLGL_DISABLE := coreal
include lib/flgl/flgl.mk
```

## Platforms

- **macOS / Linux**: system compiler + `make`. Nothing else.
- **Windows**: build under MSYS2 with MinGW `gcc` + `make` (no cmake, no MSVC).
  Same commands from the MSYS2 shell.
