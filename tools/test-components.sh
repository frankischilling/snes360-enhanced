#!/usr/bin/env bash
set -eu
cd "$(dirname "$0")/.."
build_dir=$(mktemp -d)
trap 'rm -rf "$build_dir"' EXIT
compile() {
    if ! g++ -std=c++03 -Wall -Wextra -fsanitize=address,undefined "$@" >"$build_dir/compiler.log" 2>&1; then
        cat "$build_dir/compiler.log"
        exit 1
    fi
}
compile -Itests/shims -Isnes9x-1.51-src-d3d/win32 \
    tests/frontend_test.cpp snes9x-1.51-src-d3d/win32/RomLaunch.cpp -o "$build_dir/frontend"
"$build_dir/frontend"
compile -Itests/shims -Isnes9x-1.51-src-d3d/win32 -Isnes9x-1.51-src-d3d \
    tests/favorites_test.cpp snes9x-1.51-src-d3d/win32/RomSettings.cpp \
    snes9x-1.51-src-d3d/TinyXML/tinystr.cpp snes9x-1.51-src-d3d/TinyXML/tinyxml.cpp \
    snes9x-1.51-src-d3d/TinyXML/tinyxmlparser.cpp snes9x-1.51-src-d3d/TinyXML/tinyxmlerror.cpp \
    -o "$build_dir/settings"
(cd "$build_dir" && ./settings)
compile -DHAVE_STDINT_H -ffunction-sections -fdata-sections \
    -Itests/shims -Isnes9x-1.51-src-d3d -Isnes9x-1.51-src-d3d/win32 \
    tests/multitap_test.cpp snes9x-1.51-src-d3d/controls.cpp \
    -Wl,--gc-sections -o "$build_dir/multitap"
"$build_dir/multitap"
