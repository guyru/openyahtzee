; installer.nsi
;
; This is the NSIS installer script for Open Yahtzee. 
;
;
; 
;--------------------------------

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------

; The name of the installer
Name "OpenYahtzee 1.5.1"

; The file to write
OutFile "OpenYahtzee-1.5.1.exe"

; The default installation directory
InstallDir $PROGRAMFILES\OpenYahtzee

;hides details of the install and uninstall but allows the users to see them if he wants
ShowInstDetails hide
ShowUninstDetails hide

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\OpenYahtzee" "Install_Dir"

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "COPYING.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------

; The stuff to install
Section "OpenYahtzee-1.5.1 (required)" SecOpenYahtzee

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "openyahtzee.exe"
  File "mingwm10.dll"
  File "wxbase28_gcc_custom.dll"
  File "wxmsw28_core_gcc_custom.dll"
  File "README.txt"
  File "COPYING.txt"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\OpenYahtzee "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenYahtzee" "DisplayName" "OpenYahtzee"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenYahtzee" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenYahtzee" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenYahtzee" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts" SecStartMenu

  CreateDirectory "$SMPROGRAMS\OpenYahtzee"
  CreateShortCut "$SMPROGRAMS\OpenYahtzee\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\OpenYahtzee\OpenYahtzee.lnk" "$INSTDIR\openyahtzee.exe" "" "$INSTDIR\openyahtzee.exe" 0
  
SectionEnd
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecOpenYahtzee ${LANG_ENGLISH} "The OpenYahtzee game files and the required libraries."
  LangString DESC_SecStartMenu ${LANG_ENGLISH} "Create shortcuts in the Start menu"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecOpenYahtzee} $(DESC_SecOpenYahtzee)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecStartMenu} $(DESC_SecStartMenu)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenYahtzee"
  DeleteRegKey HKLM SOFTWARE\OpenYahtzee

  ; Remove files and uninstaller
  Delete $INSTDIR\openyahtzee.exe
  Delete "wxbase28_gcc_custom.dll"
  Delete "wxmsw28_core_gcc_custom.dll"
  Delete "COPYING.txt"
  Delete "README.txt"
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\OpenYahtzee\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\OpenYahtzee"
  RMDir "$INSTDIR"

SectionEnd
