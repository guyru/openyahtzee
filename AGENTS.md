# AGENTS.md

This file provides guidance to AI coding agents when working with code in this repository.

## Project Overview

Open Yahtzee is a free, open-source implementation of the classic Yahtzee dice game. It is a C++17 desktop application using wxWidgets for the GUI. Licensed under GPL v2+.

## Build System

Builds use CMake and run inside containers (podman/docker) to avoid polluting the host with build dependencies. **Always use the container-based build commands below for building and packaging unless the user specifically asks otherwise.**

### Linux Build

```bash
# Build the container image (one-time)
podman build -t openyahtzee-build .

# Build
mkdir -p build
podman run --rm -v "$(pwd):/src:Z" -v "$(pwd)/build:/build:Z" openyahtzee-build \
  sh -c "cmake -B /build -S /src && cmake --build /build"
```

Output: `build/src/openyahtzee`

### Windows Portable Build (cross-compiled from Linux)

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

Output: `build-win/src/openyahtzee.exe` (statically linked, no DLL dependencies)

### CMake Options

- `-DPORTABLE=ON` — Portable edition: stores config alongside the executable instead of in the user's home directory. Used for the Windows portable build.

### Dependencies

- **Linux** (`Dockerfile`): CMake, g++, wxWidgets 3.2 dev, gettext
- **Windows cross-compile** (`Dockerfile.mingw`): CMake, mingw-w64 (posix threading), wxWidgets 3.2 (built from source with `--disable-shared`)
- **Portable deb** (`Dockerfile.deb`): CMake, g++, wxWidgets 3.2 (built from source with `--disable-shared`), GTK3 dev, dpkg-dev, debhelper
- Cross-compilation toolchain file: `cmake/mingw-w64-x86_64.cmake`

### Website

The project website (in `website/`) uses Pelican (Python static site generator) and deploys to GitHub Pages via `.github/workflows/deploy-website.yml`.

```bash
cd website
pip install pelican markdown
pelican content -s pelicanconf.py    # local dev
pelican content -s publishconf.py    # production build
```

## Architecture

The application is a single-window wxWidgets app. Key classes:

- **`MyApp`** (`src/openyahtzee.cpp`) — wxWidgets application entry point. Sets up locale/i18n and creates the main frame.
- **`MainFrame`** (`src/MainFrame.{h,cpp}`) — The main game window. Contains all game state, UI layout, dice display, score buttons, and event handling for gameplay. This is the largest and most central file.
- **`ScoreDice`** (`src/ScoreDice.{h,cpp}`) — Yahtzee scoring logic. Given 5 dice values, computes scores for all categories (aces through chance, including full house, straights, yahtzee). Also handles Yahtzee joker rules.
- **`Configuration`** (`src/configuration.{h,cpp}`) — Reads/writes a custom config file format storing settings and high scores as key-value pairs.
- **`Statistics`** (`src/statistics.{h,cpp}`) — Tracks game statistics (games started/finished, score distribution) persisted via Configuration.
- **`wxDynamicBitmap`** (`src/wxDynamicBitmap.{h,cpp}`) — Custom wxWidgets control that displays bitmaps with immediate drawing (unlike wxStaticBitmap) and supports grayscale conversion.

Dice face images are embedded as XPM files (`one.xpm` through `six.xpm`).

## Packaging

### Windows zip

After cross-compiling, run `cpack` inside the build directory:

```bash
podman run --rm -v "$(pwd):/src:Z" -v "$(pwd)/build-win:/build:Z" openyahtzee-mingw \
  sh -c "cmake -B /build -S /src \
    -DCMAKE_TOOLCHAIN_FILE=/src/cmake/mingw-w64-x86_64.cmake \
    -DPORTABLE=ON \
    -DwxWidgets_CONFIG_EXECUTABLE=/usr/x86_64-w64-mingw32/bin/wx-config \
    && cmake --build /build && cd /build && cpack"
```

Output: `build-win/openyahtzee-<version>.zip`

### Debian package

Debian packaging files are in `debian/`. The package uses `debhelper` with the CMake buildsystem and installs the binary to `/usr/games`.

To build inside the Linux container (which lacks `dpkg-buildpackage`), install the tools first:

```bash
podman run --rm \
  -v "$(pwd):/src/openyahtzee:Z" \
  -v "/tmp/deb-output:/src:Z" \
  openyahtzee-build \
  sh -c "apt-get update -qq && apt-get install -y -qq dpkg-dev debhelper fakeroot \
    && cd /src/openyahtzee && dpkg-buildpackage -us -uc -b"
```

Output: `/tmp/deb-output/openyahtzee_<version>_amd64.deb`

Note: mount the source at `/src/openyahtzee` (not `/src`) so that `dpkg-buildpackage` can write output files to the parent directory `/src`, which must also be a mounted volume.

### Debian package (portable, static wxWidgets)

For a .deb that works across multiple Debian/Ubuntu versions, use `Dockerfile.deb` which builds wxWidgets from source as a static library. The resulting binary links wxWidgets statically while keeping GTK3 and other system libraries dynamic, eliminating the wxWidgets shared library version dependency.

```bash
# Build the deb container image (one-time, ~10min — compiles wxWidgets from source)
podman build -f Dockerfile.deb -t openyahtzee-deb .

# Build the .deb package
podman run --rm \
  -v "$(pwd):/src/openyahtzee:Z" \
  -v "/tmp/deb-output:/src:Z" \
  openyahtzee-deb \
  sh -c "cd /src/openyahtzee && dpkg-buildpackage -d -us -uc -b"
```

Output: `/tmp/deb-output/openyahtzee_<version>_amd64.deb`

The `-d` flag skips the Build-Depends check since `libwxgtk3.2-dev` is replaced by the static build inside the container.

### Version

The single source of truth for the version number is `CMakeLists.txt` line 2 (`project(... VERSION x.y ...)`). This flows automatically to the about dialog via the `VERSION` compile definition. Other files that contain version info and need manual updates on release:

- `CHANGELOG.md` — release notes
- `debian/changelog` — Debian package changelog
- `website/content/wiki/download.md` — download links and checksums
- `website/content/wiki/01_index.md` — release announcement

## i18n

Translations use GNU gettext. Translation files are in `po/`. Currently has Hebrew (`he.po`). The translatable strings catalog is `po/openyahtzee.pot`, and `po/POTFILES.in` lists source files with translatable strings.

To update the `.pot` template after changing translatable strings:

```bash
xgettext --keyword=_ --keyword=N_ -o po/openyahtzee.pot $(cat po/POTFILES.in)
```

To update an existing translation:

```bash
msgmerge --update po/he.po po/openyahtzee.pot
```
