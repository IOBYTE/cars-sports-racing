;
;        Windows NSIS configuration file for the "Unmaintained contents" installer
;        (obsolete physics engines, ...)
;
;        copyright  : (C) 2012 onwards Jean-Philippe Meuret
;        $Id: ca36GP-sports-racing-unmaintained.nsi 7159 2020-07-31 13:40:13Z beaglejoe $
;
;        This program is free software; you can redistribute it and/or modify
;        it under the terms of the GNU General Public License as published by
;        the Free Software Foundation; either version 2 of the License, or
;        (at your option) any later version.

; Common definitions.
!include "cars-sports-racing.nsh"

; MUI Settings
!define MUI_WELCOMEPAGE_TITLE "${GAME_NAME}\n     ${GAME_SUBTITLE}\nSeason 5"
!define MUI_WELCOMEPAGE_TITLE_3LINES

; Installer pages
;Page custom EnterXXXPage LeaveXXXPage
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_INSTFILES

; Language files (Warning: Can't be upper in the script)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "Danish"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Finnish"
!insertmacro MUI_LANGUAGE "Norwegian"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Slovak"
!insertmacro MUI_LANGUAGE "Hungarian"
!insertmacro MUI_LANGUAGE "Romanian"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Arabic"
!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "SimpChinese"

; Global installer settings.
Name "${GAME_VERSIONNED_NAME}"
Caption "Install SEASON 5 tracks, cars & AI drivers datas for ${GAME_VERSIONNED_NAME}"
OutFile "${OUT_PATH}\${GAME_SHORT_FS_NAME}-${SEASON5_SETUP_KEY}-${GAME_LONG_VERSION}-win32-setup.exe"
InstallDir "$PROGRAMFILES\${INST_INST_DIR_SUFFIX}" ; TODO: Detect base package install dir
InstallDirRegKey ${GAME_ROOT_REGKEY} "${GAME_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section -CoreData

   SetOverwrite on

  ; Game core data files (racemanagers, ...)
  SetOutPath "$INSTDIR\data\config\raceman"
  File "${BUILD_INST_DIR}\data\config\raceman\LS1-practice.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS1-challenge1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS1-challenge2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS1-championship1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS1-championship2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS1-championship3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS1-endurance1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS1-endurance2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS1-endurance3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS1-endurance4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\36GP-practice.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\36GP-challenge1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\36GP-challenge2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\36GP-championship1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\36GP-championship2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\36GP-championship3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\36GP-endurance1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\36GP-endurance2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\36GP-endurance3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\36GP-endurance4.xml"

SectionEnd

Section -Drivers

  SetOverwrite on

  ; Customizable robots binaries
  SetOutPath "$INSTDIR\lib\drivers\axiom_ls1"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\axiom_ls1\*.*"

  SetOutPath "$INSTDIR\lib\drivers\shadow_ls1"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\shadow_ls1\*.*"

  SetOutPath "$INSTDIR\lib\drivers\usr_ls1"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\usr_ls1\*.*"

  SetOutPath "$INSTDIR\lib\drivers\axiom_36GP"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\axiom_36GP\*.*"

  ; Customizable robot data files (car and track setups)
  SetOutPath "$INSTDIR\data\drivers\axiom_ls1"
  File /r "${BUILD_INST_DIR}\data\drivers\axiom_ls1\*.*"

  ; Robots data files (car and track setups)
  SetOutPath "$INSTDIR\data\drivers\shadow_ls1"
  File /r "${BUILD_INST_DIR}\data\drivers\shadow_ls1\*.*"

  SetOutPath "$INSTDIR\data\drivers\usr_ls1"
  File /r "${BUILD_INST_DIR}\data\drivers\usr_ls1\*.*"

  SetOutPath "$INSTDIR\data\drivers\axiom_36GP"
  File /r "${BUILD_INST_DIR}\data\drivers\axiom_36GP\*.*"

SectionEnd

Section -Cars

  SetOverwrite on
  
  ; Season 5 / LS1 & 36GP car models
  SetOutPath "$INSTDIR\data\cars\models\ls1-archer-r9"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls1-archer-r9\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls1-cavallo-570s1"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls1-cavallo-570s1\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls1-marisatech-gt4r"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls1-marisatech-gt4r\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls1-newcastle-fury"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls1-newcastle-fury\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls1-taipan-ltsr"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls1-taipan-ltsr\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls1-toro-rgt"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls1-toro-rgt\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls1-vulture-v5r"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls1-vulture-v5r\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls1-vulture-v6r"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls1-vulture-v6r\*.*"
  
  SetOutPath "$INSTDIR\data\cars\models\ls1-zentek-z7r"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls1-zentek-z7r\*.*"

  SetOutPath "$INSTDIR\data\cars\models\36gp-century-r11b"
  File /r "${BUILD_INST_DIR}\data\cars\models\36gp-century-r11b\*.*"

  SetOutPath "$INSTDIR\data\cars\models\36gp-ettore-t59"
  File /r "${BUILD_INST_DIR}\data\cars\models\36gp-ettore-t59\*.*"

  SetOutPath "$INSTDIR\data\cars\models\36gp-milano-12c36"
  File /r "${BUILD_INST_DIR}\data\cars\models\36gp-milano-12c36\*.*"

  SetOutPath "$INSTDIR\data\cars\models\36gp-milano-tipop3"
  File /r "${BUILD_INST_DIR}\data\cars\models\36gp-milano-tipop3\*.*"

  SetOutPath "$INSTDIR\data\cars\models\36gp-motorunion-typec"
  File /r "${BUILD_INST_DIR}\data\cars\models\36gp-motorunion-typec\*.*"

  SetOutPath "$INSTDIR\data\cars\models\36gp-silber-w25b"
  File /r "${BUILD_INST_DIR}\data\cars\models\36gp-silber-w25b\*.*"

  SetOutPath "$INSTDIR\data\cars\models\36gp-tridenti-6c34"
  File /r "${BUILD_INST_DIR}\data\cars\models\36gp-tridenti-6c34\*.*"

  SetOutPath "$INSTDIR\data\cars\models\36gp-tridenti-v8ri"
  File /r "${BUILD_INST_DIR}\data\cars\models\36gp-tridenti-v8ri\*.*"

SectionEnd

Section -Tracks

  SetOverwrite on
  
    ; Season 5 tracks
  SetOutPath "$INSTDIR\data\tracks\circuit\bueno"
  File /r "${BUILD_INST_DIR}\data\tracks\circuit\bueno\*.*"
    
  SetOutPath "$INSTDIR\data\tracks\circuit\nordschleife"
  File /r "${BUILD_INST_DIR}\data\tracks\circuit\nordschleife\*.*"

  SetOutPath "$INSTDIR\data\tracks\road\pinabashi"
  File /r "${BUILD_INST_DIR}\data\tracks\road\pinabashi\*.*"

  SetOutPath "$INSTDIR\data\tracks\road\street-1"
  File /r "${BUILD_INST_DIR}\data\tracks\road\street-1\*.*"

  SetOutPath "$INSTDIR\data\tracks\road\lombaro"
  File /r "${BUILD_INST_DIR}\data\tracks\road\lombaro\*.*"

  SetOutPath "$INSTDIR\data\tracks\grandprix\ardennen-spa"
  File /r "${BUILD_INST_DIR}\data\tracks\grandprix\ardennen-spa\*.*"

SectionEnd

; This function is called at installer startup.
Function .onInit

  ; Check if the base package is installed and OK, and abort if not.
  !insertmacro CheckIfBasePackageInstalled

FunctionEnd

