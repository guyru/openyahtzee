; installer.nsi
;
; This is the NSIS installer script for Open Yahtzee. 
;
;
; 
;--------------------------------
!define version 1.9
;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------

; The name of the installer
Name "Open Yahtzee ${version}"

; The file to write
OutFile "openyahtzee-${version}.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\Open Yahtzee"

;hides details of the install and uninstall but allows the users to see them if he wants
ShowInstDetails hide
ShowUninstDetails hide

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Open Yahtzee" "Install_Dir"

; set the icon for installer
;Icon "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

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
Section "Open Yahtzee-${version} (required)" SecOpenYahtzee

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "openyahtzee.exe"
  File "COPYING.txt"
  File "ChangeLog.txt"

  ; Copy the runtime dlls
  SetOutPath "$INSTDIR\Microsoft.VC90.CRT"
  File "Microsoft.VC90.CRT\*.dll"
  File "Microsoft.VC90.CRT\*.manifest"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "Software\Open Yahtzee" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Open Yahtzee" "DisplayName" "Open Yahtzee"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Open Yahtzee" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Open Yahtzee" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Open Yahtzee" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts" SecStartMenu
  ; Remove old shortcuts, if any
  Delete "$SMPROGRAMS\OpenYahtzee\*.*"
  Delete "$SMPROGRAMS\Open Yahtzee\*.*"

  CreateDirectory "$SMPROGRAMS\Open Yahtzee"
  CreateShortCut "$SMPROGRAMS\Open Yahtzee\Open Yahtzee.lnk" "$INSTDIR\openyahtzee.exe" "" "$INSTDIR\openyahtzee.exe" 0
  CreateShortCut "$SMPROGRAMS\Open Yahtzee\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  
SectionEnd
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecOpenYahtzee ${LANG_ENGLISH} "The Open Yahtzee game files and the required libraries."
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
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Open Yahtzee"
  DeleteRegKey HKLM "SOFTWARE\Open Yahtzee"

  ; Remove files and uninstaller
  Delete "$INSTDIR\openyahtzee.exe"
  Delete "$INSTDIR\COPYING.txt"
  Delete "$INSTDIR\ChangeLog.txt"
  Delete "$INSTDIR\uninstall.exe"
  Delete "$INSTDIR\Microsoft.VC90.CRT\*.*"
  ; Remove old files
  Delete "$INSTDIR\openyahtzee.exe.manifest"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\icon32.ico"

  RMDir "$INSTDIR\Microsoft.VC90.CRT"
  Delete "$INSTDIR\*"


  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Open Yahtzee\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Open Yahtzee"
  RMDir "$INSTDIR"

SectionEnd
