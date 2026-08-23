#!/bin/sh
# flgl user-flow testbench — end-to-end test of the real consumer flow, run by
# hand on macOS / Linux / MSYS2. see usage() below.
set -eu

usage() {
	cat <<'EOF'
usage: ./run.sh <branch>

end-to-end test of the flgl user flow on this OS. for the given branch it:
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
	"")        echo "run.sh: missing <branch>" >&2; echo >&2; usage >&2; exit 2 ;;
esac
branch="$1"

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

	git init -q
	git submodule add -q -b "$branch" "$FLGL_URL" lib/flgl
	git submodule update -q --init --recursive
	./lib/flgl/user/setup.sh
	make DISABLE="$disable"

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
