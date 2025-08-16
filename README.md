# C++23 + LLVM/Clang 19 + CMake Starter

## Prerequisites (Ubuntu 24.04 AMD64)
- LLVM/Clang 19: `sudo apt install clang-19 lldb-19 lld-19 clang-tidy-19 clang-format-19`
- CMake ≥ 3.26: `sudo apt install cmake ninja-build`
- Optional: `ccache`

## Build
```bash
./build.sh            # Debug default
./build.sh --release  # Release
./build.sh --werror   # Treat warnings as errors
```
Artifacts in `build/`. `compile_commands.json` is symlinked to project root.

## Run
```bash
./build/starter_app
```

## Tooling
- Formatting: `.clang-format` (Google style, C++23)
- Static analysis: `.clang-tidy`, `.clangd`
- VSCode/Cursor: `.vscode/` settings and extensions recommendations
- Git: `.gitignore`, `.editorconfig`