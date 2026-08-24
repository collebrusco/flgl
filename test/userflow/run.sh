#!/bin/sh
# flgl user-flow testbench — end-to-end test of the real consumer flow, run by
# hand on macOS / Linux / MSYS2. see usage() below.
set -eu

usage() {
	cat <<'EOF'
usage: ./run.sh <branch>

  clones flgl from GitHub, runs user/setup.sh in a fresh throwaway project, and
  builds the hello-world twice — with all modules, and with audio off
  (DISABLE=coreal). scratch is left under $TMPDIR for inspection.

  the real flow it mirrors:
    git init ; git submodule add flgl lib/flgl ; ./lib/flgl/user/setup.sh ; make

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
	echo "=== config: $name   (DISABLE='$disable') ============================="
	mkdir -p "$dir"
	cd "$dir"

	echo "--> git init + submodule add flgl@$branch (clones glfw/glm/miniaudio, ~slow)"
	git init -q
	git submodule add -b "$branch" "$FLGL_URL" lib/flgl
	git submodule update --init --recursive
	echo "--> ./lib/flgl/user/setup.sh"
	./lib/flgl/user/setup.sh
	echo "--> make DISABLE='$disable'"
	make -j DISABLE="$disable"

	# MinGW appends .exe; accept either name.
	if [ -x build/untitled ] || [ -x build/untitled.exe ]; then
		echo "[ok]   $name: built"
	else
		echo "[FAIL] $name: build/untitled not produced"
		return 1
	fi
}

rc=0
run_config full     ""       || rc=1
run_config no-audio "coreal" || rc=1

echo
echo "[testbench] scratch left at: $work  (rm -rf to clean)"
if [ "$rc" -eq 0 ]; then
	echo "[testbench] RESULT: all configs passed"
else
	echo "[testbench] RESULT: FAILURES above"
fi
exit "$rc"
