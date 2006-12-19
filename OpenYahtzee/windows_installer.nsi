; installer.nsi
;
; This is the NSIS installer script for Open Yahtzee. 
;
;
; 
;--------------------------------

; The name of the installer
Name "Open Yahtzee 1.5.1"

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

;make the installer controls use the new XP style when running on Windows XP
XPStyle on

;set the license
LicenseData COPYING.txt

;--------------------------------

; Pages

Page license
Page components
Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles


;--------------------------------

; The stuff to install
Section "OpenYahtzee-1.5.1 (required)"

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
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\OpenYahtzee"
  CreateShortCut "$SMPROGRAMS\OpenYahtzee\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\OpenYahtzee\OpenYahtzee.lnk" "$INSTDIR\openyahtzee.exe" "" "$INSTDIR\openyahtzee.exe" 0
  
SectionEnd

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
