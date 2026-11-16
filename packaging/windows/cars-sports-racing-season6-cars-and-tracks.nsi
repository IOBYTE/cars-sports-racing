;
;        Windows NSIS configuration file for the "Unmaintained contents" installer
;        (obsolete physics engines, ...)
;
;        copyright  : (C) 2012 onwards Jean-Philippe Meuret
;        $Id: ca67GP-sports-racing-unmaintained.nsi 7159 2020-07-31 13:40:13Z beaglejoe $
;
;        This program is free software; you can redistribute it and/or modify
;        it under the terms of the GNU General Public License as published by
;        the Free Software Foundation; either version 2 of the License, or
;        (at your option) any later version.

; Common definitions.
!include "cars-sports-racing.nsh"

; MUI Settings
!define MUI_WELCOMEPAGE_TITLE "${GAME_NAME}\n     ${GAME_SUBTITLE}\nSeason 6"
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
Caption "Install SEASON 6 tracks, cars & AI drivers datas for ${GAME_VERSIONNED_NAME}"
OutFile "${OUT_PATH}\${GAME_SHORT_FS_NAME}-${SEASON6_SETUP_KEY}-${GAME_LONG_VERSION}-win32-setup.exe"
InstallDir "$PROGRAMFILES\${INST_INST_DIR_SUFFIX}" ; TODO: Detect base package install dir
InstallDirRegKey ${GAME_ROOT_REGKEY} "${GAME_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section -CoreData

   SetOverwrite on

  ; Game core data files (racemanagers, ...)
  SetOutPath "$INSTDIR\data\config\raceman"
  File "${BUILD_INST_DIR}\data\config\raceman\LP2-practice.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LP2-challenge1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LP2-challenge2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LP2-championship1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LP2-championship2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LP2-championship3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LP2-endurance1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LP2-endurance2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LP2-endurance3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LP2-endurance4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\67GP-practice.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\67GP-challenge1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\67GP-challenge2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\67GP-championship1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\67GP-championship2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\67GP-championship3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\67GP-endurance1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\67GP-endurance2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\67GP-endurance3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\67GP-endurance4.xml"

SectionEnd

Section -Drivers

  SetOverwrite on

  ; Customizable robots binaries
  SetOutPath "$INSTDIR\lib\drivers\axiom_lp2"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\axiom_lp2\*.*"

  SetOutPath "$INSTDIR\lib\drivers\shadow_lp2"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\shadow_lp2\*.*"

  SetOutPath "$INSTDIR\lib\drivers\usr_lp2"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\usr_lp2\*.*"

  SetOutPath "$INSTDIR\lib\drivers\axiom_67GP"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\axiom_67GP\*.*"

  ; Customizable robot data files (car and track setups)
  SetOutPath "$INSTDIR\data\drivers\axiom_lp2"
  File /r "${BUILD_INST_DIR}\data\drivers\axiom_lp2\*.*"

  ; Robots data files (car and track setups)
  SetOutPath "$INSTDIR\data\drivers\shadow_lp2"
  File /r "${BUILD_INST_DIR}\data\drivers\shadow_lp2\*.*"

  SetOutPath "$INSTDIR\data\drivers\usr_lp2"
  File /r "${BUILD_INST_DIR}\data\drivers\usr_lp2\*.*"

  SetOutPath "$INSTDIR\data\drivers\axiom_67GP"
  File /r "${BUILD_INST_DIR}\data\drivers\axiom_67GP\*.*"

SectionEnd

Section -Cars

  SetOverwrite on
  
  ; Season 6 / LP2 & 67GP car models
  SetOutPath "$INSTDIR\data\cars\models\lp2-bravery-brc69"
  File /r "${BUILD_INST_DIR}\data\cars\models\lp2-bravery-brc6\*.*"

  SetOutPath "$INSTDIR\data\cars\models\lp2-lolita-exr57"
  File /r "${BUILD_INST_DIR}\data\cars\models\lp2-lolita-exr57\*.*"

  SetOutPath "$INSTDIR\data\cars\models\lp2-spire-s101"
  File /r "${BUILD_INST_DIR}\data\cars\models\lp2-spire-s101\*.*"

  SetOutPath "$INSTDIR\data\cars\models\67gp-cavallo-123"
  File /r "${BUILD_INST_DIR}\data\cars\models\67gp-cavallo-123\*.*"

  SetOutPath "$INSTDIR\data\cars\models\67gp-guthrie-eagle"
  File /r "${BUILD_INST_DIR}\data\cars\models\67gp-guthrie-eagle\*.*"

  SetOutPath "$INSTDIR\data\cars\models\67gp-kuiper-t81"
  File /r "${BUILD_INST_DIR}\data\cars\models\67gp-kuiper-t81\*.*"

  SetOutPath "$INSTDIR\data\cars\models\67gp-mango-ms7"
  File /r "${BUILD_INST_DIR}\data\cars\models\67gp-mango-ms7\*.*"

  SetOutPath "$INSTDIR\data\cars\models\67gp-murasama-37a"
  File /r "${BUILD_INST_DIR}\data\cars\models\67gp-murasama-37a\*.*"

  SetOutPath "$INSTDIR\data\cars\models\67gp-upping-ut19"
  File /r "${BUILD_INST_DIR}\data\cars\models\67gp-upping-ut19\*.*"

SectionEnd

Section -Tracks

  SetOverwrite on
  
    ; Season 6 tracks
  SetOutPath "$INSTDIR\data\tracks\road\spring"
  File /r "${BUILD_INST_DIR}\data\tracks\road\spring\*.*"

SectionEnd

; This function is called at installer startup.
Function .onInit

  ; Check if the base package is installed and OK, and abort if not.
  !insertmacro CheckIfBasePackageInstalled

FunctionEnd

