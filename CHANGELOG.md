# Changelog

All notable changes to Open Yahtzee are documented in this file.

## [Unreleased]

- Replace the raster app icon with an SVG; raster icons are rendered from it at build time (new build dependency: `rsvg-convert`).
- Install the icon to `share/icons/hicolor/scalable/apps` instead of `share/pixmaps`.
- Disable the Roll button when all five dice are kept, so no roll is wasted.

## [1.10]

- Modernize build system: migrate to CMake, drop Boost, target C++17.
- Require wxWidgets 3.2.
- Fix dice animation on Linux and improve RNG.
- Fix the about dialog display.
- Fix Windows executable icon.
- Add unit tests.

## [1.9.3]

- wxWidgets 3.0 compatibility.
- Windows compatibility fixes.
- GUI layout fixes for wxWidgets 3.0.

## [1.9.2]

- wxWidgets 3.0 compatibility.
- Bug fix: menu-bar sizing issues in Ubuntu with wxGTK 2.8.
- Bug fix: improper size after changing layout.

## [1.9.1]

- Make the 2s dice more distinguishable from the 3s dice.
- Install a basic man page.
- Update desktop file.
- Use Mersenne Twister from C++11 instead of rand().

## [1.9]

- Show scoring tip on mouse over.
- Immediately apply the bonus for the upper section when 63 points are reached.
- Display the number of rolls left.
- Add link to the FAQ and HowToPlay pages from the Open Yahtzee website.
- Improved randomness of the dice.
- Better Windows Vista compatibility.
- Improved autoconf package.
- Fixed bug #1775620 (Settings dialog enables roll button).
- Fixed window title caption (bug #1836473).
- Fixed the display of the About dialog.
- Simple text-based settings storage.
- Major code refactoring.

## [1.8]

- New horizontal layout for the user interface, available as an option.
- Clicking on a die sets the "keep" checkbox.
- Kept dice are now shown in grayscale.
- Realigned the "keep" checkboxes to the dice.
- Fixed bug #1700729 (undoing the first move of the game).
- Fixed bug #1719068 (UI behavior of the dialog asking for the user's name).
- Fixed bug #1719063 (double-clicking on the roll button).
- Fixed bug #1731178 (undoing Yahtzee still qualifies for Yahtzee bonus).
- Refactored the dialog that asks for the user's name for the high score table.
- Redesigned About dialog.
- Reduced installer and binary size for Windows.
- Better integration with Windows XP themes.

## [1.7]

- New dice graphics.
- Dice rolling animation.
- Fixed bug #1634542 (undoing scoring that involves Yahtzee bonus).
- Fixed minor misalignment in the UI.
- Code refactoring and cleanup.
- Redesigned Settings dialog.
- The main window is now non-resizable.
- Disabled the ability to undo the last move of the game.
- Implemented Yahtzee joker scoring rules.
- Added accelerator key for the Undo button (Ctrl+Z).
- Added ability to send comments to the developer.
- Calculates the sub-total score after every score (can be disabled).
- New icons for the Windows installer.

## [1.6]

- Added an Undo button.
- Improved Windows portability.
- New icons for the program.
- Ability to check for updates via the web.
- Added database initialization with fallback to default values.
- Code refactoring and cleanup.
- Fixed typos in high-score related dialogs.
