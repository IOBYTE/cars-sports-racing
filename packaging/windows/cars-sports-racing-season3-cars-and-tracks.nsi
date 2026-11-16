;
;        Windows NSIS configuration file for the "HQ Cars & Tracks (1)" installer
;        (HQ tracks and cars with full AI support)
;
;        copyright  : (C) 2011 onwards Jean-Philippe Meuret
;        $Id: cars-sports-racing-hq-cars-and-tracks.nsi 8281 2022-06-29 17:05:26Z beaglejoe $
;
;        This program is free software; you can redistribute it and/or modify
;        it under the terms of the GNU General Public License as published by
;        the Free Software Foundation; either version 2 of the License, or
;        (at your option) any later version.

; Common definitions.
!include "cars-sports-racing.nsh"

;SetCompressor /SOLID lzma ; 1-block compression, smaller but longer (generation, extraction)

; MUI Settings
!define MUI_WELCOMEPAGE_TITLE "${GAME_NAME}\n     ${GAME_SUBTITLE}\nSEASON 3 (1) tracks, cars and AI Drivers datas"
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
Caption "Install SEASON 3 tracks, cars & AI drivers datas for ${GAME_VERSIONNED_NAME}"
OutFile "${OUT_PATH}\${GAME_SHORT_FS_NAME}-${SEASON3_SETUP_KEY}-${GAME_LONG_VERSION}-win32-setup.exe"
InstallDir "$PROGRAMFILES\${INST_INST_DIR_SUFFIX}"
InstallDirRegKey ${GAME_ROOT_REGKEY} "${GAME_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section -CoreData

  SetOverwrite on

  ; Game core data files (racemanagers, ...)
  SetOutPath "$INSTDIR\data\config\raceman"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-practice.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-challenge1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-challenge2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-challenge3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-challenge4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-challenge5.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-challenge6.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-challenge7.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-championship1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-championship2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-championship3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-endurance1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-endurance2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-endurance3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-endurance4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-contract1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-contract2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-contract3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-contract4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-special1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-special2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-special3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\91GP-special4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-practice.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-challenge1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-challenge2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-challenge3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-challenge4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-challenge5.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-challenge6.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-challenge7.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-championship1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-championship2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-championship3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-endurance1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-endurance2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-endurance3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-endurance4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-contract1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-contract2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-contract3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-contract4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-special1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-special2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-special3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-A-special4.xml"

SectionEnd

Section -Drivers

  SetOverwrite on

  ; Customizable robots binaries
  SetOutPath "$INSTDIR\lib\drivers\axiom_sca"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\axiom_sca\*.*"

  SetOutPath "$INSTDIR\lib\drivers\axiom_91GP"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\axiom_91GP\*.*"

  ; Customizable robot data files (car and track setups)
  SetOutPath "$INSTDIR\data\drivers\axiom_sca"
  File /r "${BUILD_INST_DIR}\data\drivers\axiom_sca\*.*"

  ; Robots data files (car and track setups)
  SetOutPath "$INSTDIR\data\drivers\axiom_91GP"
  File /r "${BUILD_INST_DIR}\data\drivers\axiom_91GP\*.*"

SectionEnd

Section -Cars

  SetOverwrite on

  ; Season 3 / AI driven 12GPA & SC-B car models
  SetOutPath "$INSTDIR\data\cars\models\91gp-boxer-amani"
  File /r "${BUILD_INST_DIR}\data\cars\models\91gp-boxer-amani\*.*"

  SetOutPath "$INSTDIR\data\cars\models\91gp-cavallo-642"
  File /r "${BUILD_INST_DIR}\data\cars\models\91gp-cavallo-642\*.*"

  SetOutPath "$INSTDIR\data\cars\models\91gp-deckard-ags01"
  File /r "${BUILD_INST_DIR}\data\cars\models\91gp-deckard-ags01\*.*"

  SetOutPath "$INSTDIR\data\cars\models\91gp-deckard-b190"
  File /r "${BUILD_INST_DIR}\data\cars\models\91gp-deckard-b190\*.*"

  SetOutPath "$INSTDIR\data\cars\models\91gp-diamond-fw14"
  File /r "${BUILD_INST_DIR}\data\cars\models\91gp-diamond-fw14\*.*"

  SetOutPath "$INSTDIR\data\cars\models\sc-lynx-220"
  File /r "${BUILD_INST_DIR}\data\cars\models\sc-lynx-220\*.*"

  SetOutPath "$INSTDIR\data\cars\models\sc-spirit-300"
  File /r "${BUILD_INST_DIR}\data\cars\models\sc-spirit-300\*.*"

  SetOutPath "$INSTDIR\data\cars\models\sc-vieringe-4s"
  File /r "${BUILD_INST_DIR}\data\cars\models\sc-vieringe-4s\*.*"

SectionEnd

Section -Tracks

  SetOverwrite on

  ; Season 2 tracks
  SetOutPath "$INSTDIR\data\tracks\road\atlanta"
  File /r "${BUILD_INST_DIR}\data\tracks\road\atlanta\*.*"

  SetOutPath "$INSTDIR\data\tracks\circuit\nordschleife"
  File /r "${BUILD_INST_DIR}\data\tracks\circuit\nordschleife\*.*"

SectionEnd

; This function is called at installer startup.
Function .onInit

  ; Check if the base package is installed and OK, and abort if not.
  !insertmacro CheckIfBasePackageInstalled

FunctionEnd

