#!/bin/sh
# run from your project root like so:
#     git submodule add <flgl-url> lib/flgl
#     ./lib/flgl/user/setup.sh
set -eu

usage() {
	cat <<'EOF'
usage: ./lib/flgl/user/setup.sh [-f]

bootstrap the current project (a repo with flgl at ./lib/flgl) to build against flgl. 
brings in a Makefile, .gitignore, and main.cpp that will build a small demo.

options:
    -f    overwrite files that already exist (default: skip them)
    -h    show this help
EOF
}

force=""
case "${1:-}" in
	-h|--help) usage; exit 0 ;;
	-f)        force="-f" ;;
	"")        ;;
	*)         echo "flgl setup: unknown argument '$1'" >&2; echo >&2; usage >&2; exit 2 ;;
esac

src="./lib/flgl/user"

# sanity: a git repo with flgl vendored at ./lib/flgl
if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
	echo "flgl setup: run this from inside your project's git repo" >&2
	exit 1
fi
if [ ! -f "$src/user_Makefile" ]; then
	echo "flgl setup: flgl not found at ./lib/flgl — add it first:" >&2
	echo "    git submodule add <flgl-url> lib/flgl" >&2
	exit 1
fi

# copy <from> <to> — skip if <to> exists, unless -f
copy() {
	if [ -e "$2" ] && [ "$force" != "-f" ]; then
		echo "  skip  $2 (exists)"
	else
		cp "$1" "$2"; echo "  wrote $2"
	fi
}

mkdir -p src
copy "$src/user_Makefile"   Makefile
copy "$src/gitignore"       .gitignore
copy "$src/helloflgl.cpp"   src/main.cpp

echo "flgl: done — 'make' to build, 'make run' to run."
