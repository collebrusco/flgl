#!/bin/sh
# flgl basic user flow test
set -eu

usage() {
	cat <<'EOF'
usage: ./run.sh <branch>

basically does this
    git init ; git submodule add flgl lib/flgl ; ./lib/flgl/user/setup.sh ; make
for a couple different build configs, makes sure the demo builds in each case.

  <branch>   flgl branch to test (must already be pushed to the remote)
  -h         show this help
EOF
}

case "${1:-}" in
	-h|--help) usage; exit 0 ;;
	"")        branch="$(git branch --show-current)" ;;
	*)         branch="$1" ;;
esac

FLGL_URL="https://github.com/collebrusco/flgl.git"
work="$(mktemp -d "${TMPDIR:-/tmp}/flgl-userflow.XXXXXX")"

echo "[testbench] remote : $FLGL_URL"
echo "[testbench] branch : $branch"
echo "[testbench] scratch: $work"

# build one config in a fresh project; assert the app comes out.
# args: <config-name> <DISABLE value>
run_config() {
	name="$1"; disable="$2"
	dir="$work/$name"
	echo
	echo "[testbench] running: $name (DISABLE='$disable')"
	mkdir -p "$dir"
	cd "$dir"

	git init -q
	git submodule add -b "$branch" "$FLGL_URL" lib/flgl
	git submodule update --init --recursive
	echo "[testbench] ./lib/flgl/user/setup.sh"
	./lib/flgl/user/setup.sh
	echo "[testbench] make -j DISABLE='$disable'"
	make -j DISABLE="$disable"

	# mingw appends .exe so accept either name.
	if [ -x build/untitled ] || [ -x build/untitled.exe ]; then
		echo "[testbench][ok]   $name: built"
	else
		echo "[testbench][FAIL] $name: build/untitled not produced"
		return 1
	fi
}

rc=0
run_config full     ""       || rc=1
run_config no-audio "coreal" || rc=1

echo
echo "[testbench] scratch left at: $work"
if [ "$rc" -eq 0 ]; then
	echo "[testbench] RESULT: all configs passed"
else
	echo "[testbench] RESULT: FAILURES"
fi
exit "$rc"
