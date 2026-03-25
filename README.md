# Open Yahtzee

A free, open-source implementation of the classic dice game Yahtzee.

Built with C++17 and [wxWidgets](https://www.wxwidgets.org/).

## Building

Builds run inside containers (podman or docker) to manage dependencies cleanly.

### Linux

```bash
# Build the container image (one-time)
podman build -t openyahtzee-build .

# Compile
mkdir -p build
podman run --rm -v "$(pwd):/src:Z" -v "$(pwd)/build:/build:Z" openyahtzee-build \
  sh -c "cmake -B /build -S /src && cmake --build /build"
```

The binary is at `build/src/openyahtzee`.

### Windows (cross-compiled from Linux)

```bash
# Build the cross-compilation image (one-time, ~10min — compiles wxWidgets from source)
podman build -f Dockerfile.mingw -t openyahtzee-mingw .

# Cross-compile
mkdir -p build-win
podman run --rm -v "$(pwd):/src:Z" -v "$(pwd)/build-win:/build:Z" openyahtzee-mingw \
  sh -c "cmake -B /build -S /src \
    -DCMAKE_TOOLCHAIN_FILE=/src/cmake/mingw-w64-x86_64.cmake \
    -DPORTABLE=ON \
    -DwxWidgets_CONFIG_EXECUTABLE=/usr/x86_64-w64-mingw32/bin/wx-config \
    && cmake --build /build"
```

The executable is at `build-win/src/openyahtzee.exe` (statically linked).

### Building without containers

Requires CMake 3.16+, a C++17 compiler, wxWidgets 3.2+, and optionally gettext.

On Debian/Ubuntu: `sudo apt-get install cmake g++ libwxgtk3.2-dev gettext`

```bash
cmake -B build
cmake --build build
cmake --install build    # optional
```

### CMake options

- `-DPORTABLE=ON` — Portable edition: stores configuration alongside the executable instead of in the user's home directory.
- `-DCMAKE_INSTALL_PREFIX=<path>` — Set installation prefix (default: `/usr/local`).

## Authors

See the `AUTHORS` file.

## License

Open Yahtzee - A free implementation of the classic dice game Yahtzee
Copyright (C) 2006-2016 Guy Rutenberg

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

See `COPYING` for the full license text.
