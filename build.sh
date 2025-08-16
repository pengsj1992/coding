#!/usr/bin/env bash
set -euo pipefail

# build.sh - Configure and build into ./build (Debug by default)
# - Prefers LLVM/Clang 19 toolchain with graceful fallback to system clang
# - Generates compile_commands.json and symlinks it to project root for tooling

BUILD_DIR="${BUILD_DIR:-build}"
BUILD_TYPE="${BUILD_TYPE:-Debug}"
JOBS="${JOBS:-$(nproc)}"
GENERATOR="${GENERATOR:-}"
WARNINGS_AS_ERRORS="${WARNINGS_AS_ERRORS:-OFF}"
CLEAN=0

usage() {
  echo "Usage: $0 [--debug|--release|--relwithdebinfo|--minsizerel] [--clean] [--jobs=N|-jN] [--werror] [--generator=Name]"
  exit 1
}

for arg in "$@"; do
  case "$arg" in
    --debug) BUILD_TYPE=Debug ;;
    --release) BUILD_TYPE=Release ;;
    --relwithdebinfo) BUILD_TYPE=RelWithDebInfo ;;
    --minsizerel) BUILD_TYPE=MinSizeRel ;;
    --clean) CLEAN=1 ;;
    --jobs=*) JOBS="${arg#*=}" ;;
    -j*) JOBS="${arg#-j}" ;;
    --werror) WARNINGS_AS_ERRORS=ON ;;
    --generator=*) GENERATOR="${arg#*=}" ;;
    --help|-h) usage ;;
    *) echo "Unknown arg: $arg" >&2; usage ;;
  esac
done

if [[ "$CLEAN" -eq 1 ]]; then
  rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"

# Pick llvm-19 if available, otherwise fallback to system clang
pick_compiler() {
  local -n out_cc=$1
  local -n out_cxx=$2
  local cc_candidates=(clang-19 clang)
  local cxx_candidates=(clang++-19 clang++)
  for c in "${cc_candidates[@]}"; do
    if command -v "$c" >/dev/null 2>&1; then out_cc="$(command -v "$c")"; break; fi
  done
  for c in "${cxx_candidates[@]}"; do
    if command -v "$c" >/dev/null 2>&1; then out_cxx="$(command -v "$c")"; break; fi
  done
  if [[ -z "${out_cc:-}" || -z "${out_cxx:-}" ]]; then
    echo "ERROR: clang/clang++ not found. Please install LLVM 19 toolchain." >&2
    exit 2
  fi
}
CC_BIN=""
CXX_BIN=""
pick_compiler CC_BIN CXX_BIN

# Detect generator if not provided: prefer Ninja when available
if [[ -z "$GENERATOR" ]]; then
  if command -v ninja >/dev/null 2>&1; then
    GENERATOR="Ninja"
  else
    GENERATOR="Unix Makefiles"
  fi
fi

# Use ccache when available
CCACHE_LAUNCHER=""
if command -v ccache >/dev/null 2>&1; then
  CCACHE_LAUNCHER="-DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache"
fi

cmake -S . -B "$BUILD_DIR" ${GENERATOR:+-G "$GENERATOR"} \
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_C_COMPILER="$CC_BIN" \
  -DCMAKE_CXX_COMPILER="$CXX_BIN" \
  -DENABLE_WARNINGS_AS_ERRORS="$WARNINGS_AS_ERRORS" \
  $CCACHE_LAUNCHER

cmake --build "$BUILD_DIR" -- -j"$JOBS"

# Symlink compile_commands.json to project root for clangd/clang-tidy
ln -sf "$BUILD_DIR/compile_commands.json" compile_commands.json

# Print toolchain info; warn if not using clang-19
compiler_version=$("$CXX_BIN" --version | head -n1)
if ! echo "$compiler_version" | grep -qE '\b19\b'; then
  echo "NOTE: Using compiler: $compiler_version (not LLVM/Clang 19)." >&2
fi

echo "Build finished: type=$BUILD_TYPE, generator=$GENERATOR, compiler=$compiler_version"
echo "Compile commands: $PWD/compile_commands.json"