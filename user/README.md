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
make                  # build against the modules listed in your Makefile
make run              # build and run (./build/untitled)
make clean
```

## How the build works

All the cross-platform knowledge — platform detection, which modules exist,
compiler/linker flags — lives in `lib/flgl/flgl.mk`. Your `Makefile` just
includes it and uses the handles it exports (`FLGL_CPPFLAGS`, `FLGL_LDLIBS`,
`FLGL_LIB`). Building your app also (re)builds `lib/flgl/build/libflgl.a` with
the matching module set.

Which flgl modules you build against is a **whitelist** near the top of your
`Makefile` — one `MODULES += <name>` line per module. Delete a line to drop that
module: its code isn't compiled or linked, and its headers go invisible (you get
a clean compile error if you use it, never a mystery link error).

```make
MODULES += coregl      # OpenGL + window
MODULES += render      # renderer
# ...delete the lines you don't need
include lib/flgl/flgl.mk
```

You can also adjust the set from the command line for a one-off build. `MODULES`
and `DISABLE` compose — `DISABLE` removes from whatever `MODULES` (or the default
of all) selects, so it works even with the whitelist above:

```sh
make MODULES="coregl render"   # build ONLY these
make DISABLE=coreal            # build the Makefile's set minus audio
```

## Platforms

- **macOS / Linux**: system compiler + `make`. Nothing else.
- **Windows**: build under MSYS2 with MinGW `gcc` + `make` (no cmake, no MSVC).
  Same commands from the MSYS2 shell.
