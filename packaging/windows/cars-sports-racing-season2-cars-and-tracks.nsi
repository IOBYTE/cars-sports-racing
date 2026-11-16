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
!define MUI_WELCOMEPAGE_TITLE "${GAME_NAME}\n     ${GAME_SUBTITLE}\nSEASON 2 (1) tracks, cars and AI Drivers datas"
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
Caption "Install SEASON 2 tracks, cars & AI drivers datas for ${GAME_VERSIONNED_NAME}"
OutFile "${OUT_PATH}\${GAME_SHORT_FS_NAME}-${SEASON2_SETUP_KEY}-${GAME_LONG_VERSION}-win32-setup.exe"
InstallDir "$PROGRAMFILES\${INST_INST_DIR_SUFFIX}"
InstallDirRegKey ${GAME_ROOT_REGKEY} "${GAME_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section -CoreData

  SetOverwrite on

  ; Game core data files (racemanagers, ...)
  SetOutPath "$INSTDIR\data\config\raceman"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-practice.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-challenge1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-challenge2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-challenge3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-challenge4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-challenge5.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-challenge6.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-challenge7.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-championship1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-championship2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-championship3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-endurance1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-endurance2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-endurance3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-endurance4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-contract1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-contract2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-contract3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-contract4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-special1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-special2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-special3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\10GP4-special4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-practice.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-challenge1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-challenge2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-challenge3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-challenge4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-challenge5.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-challenge6.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-challenge7.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-championship1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-championship2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-championship3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-endurance1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-endurance2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-endurance3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-endurance4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-contract1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-contract2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-contract3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-contract4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-special1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-special2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-special3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\SC-B-special4.xml"

SectionEnd

Section -Drivers

  SetOverwrite on

  ; Customizable robots binaries
  SetOutPath "$INSTDIR\lib\drivers\axiom_scb"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\axiom_scb\*.*"

  SetOutPath "$INSTDIR\lib\drivers\axiom_10GP4"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\axiom_10GP4\*.*"

  ; Customizable robot data files (car and track setups)
  SetOutPath "$INSTDIR\data\drivers\axiom_scb"
  File /r "${BUILD_INST_DIR}\data\drivers\axiom_scb\*.*"

  SetOutPath "$INSTDIR\data\drivers\axiom_10GP4"
  File /r "${BUILD_INST_DIR}\data\drivers\axiom_10GP4\*.*"

SectionEnd

Section -Cars

  SetOverwrite on

  ; Season 2 / AI driven 12GPA & SC-B car models
  SetOutPath "$INSTDIR\data\cars\models\10gp4-diamond"
  File /r "${BUILD_INST_DIR}\data\cars\models\10gp4-diamond\*.*"

  SetOutPath "$INSTDIR\data\cars\models\sc-boxer-96"
  File /r "${BUILD_INST_DIR}\data\cars\models\sc-boxer-96\*.*"

  SetOutPath "$INSTDIR\data\cars\models\sc-cavallo-360"
  File /r "${BUILD_INST_DIR}\data\cars\models\sc-cavallo-360\*.*"

  SetOutPath "$INSTDIR\data\cars\models\sc-fmc-gt4"
  File /r "${BUILD_INST_DIR}\data\cars\models\sc-fmc-gt4\*.*"

SectionEnd

Section -Tracks

  SetOverwrite on

  ; Season 2 tracks
  SetOutPath "$INSTDIR\data\tracks\grandprix\ardennen-spa"
  File /r "${BUILD_INST_DIR}\data\tracks\grandprix\ardennen-spa\*.*"
  
  SetOutPath "$INSTDIR\data\tracks\grandprix\corkscrew"
  File /r "${BUILD_INST_DIR}\data\tracks\grandprix\corkscrew\*.*"
  
  SetOutPath "$INSTDIR\data\tracks\grandprix\kurpfalzring"
  File /r "${BUILD_INST_DIR}\data\tracks\grandprix\kurpfalzring\*.*"

  SetOutPath "$INSTDIR\data\tracks\grandprix\melbourne"
  File /r "${BUILD_INST_DIR}\data\tracks\grandprix\melbourne\*.*"

  SetOutPath "$INSTDIR\data\tracks\grandprix\sao-paulo"
  File /r "${BUILD_INST_DIR}\data\tracks\grandprix\sao-paulo\*.*"

  SetOutPath "$INSTDIR\data\tracks\road\street-1"
  File /r "${BUILD_INST_DIR}\data\tracks\road\street-1\*.*"

SectionEnd

; This function is called at installer startup.
Function .onInit

  ; Check if the base package is installed and OK, and abort if not.
  !insertmacro CheckIfBasePackageInstalled

FunctionEnd
