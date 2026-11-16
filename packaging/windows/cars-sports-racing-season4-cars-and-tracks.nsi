;
;        Windows NSIS configuration file for the "Unmaintained contents" installer
;        (obsolete physics engines, ...)
;
;        copyright  : (C) 2012 onwards Jean-Philippe Meuret
;        $Id: cars-sports-racing-unmaintained.nsi 7159 2020-07-31 13:40:13Z beaglejoe $
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
Caption "Install SEASON 4 tracks, cars & AI drivers datas for ${GAME_VERSIONNED_NAME}"
OutFile "${OUT_PATH}\${GAME_SHORT_FS_NAME}-${SEASON4_SETUP_KEY}-${GAME_LONG_VERSION}-win32-setup.exe"
InstallDir "$PROGRAMFILES\${INST_INST_DIR_SUFFIX}" ; TODO: Detect base package install dir
InstallDirRegKey ${GAME_ROOT_REGKEY} "${GAME_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section -CoreData

   SetOverwrite on

  ; Game core data files (racemanagers, ...)
  SetOutPath "$INSTDIR\data\config\raceman"
  File "${BUILD_INST_DIR}\data\config\raceman\LS2-practice.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS2-challenge1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS2-challenge2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS2-championship1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS2-championship2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS2-championship3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS2-endurance1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS2-endurance2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS2-endurance3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\LS2-endurance4.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\RS-practice.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\RS-challenge1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\RS-challenge2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\RS-championship1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\RS-championship2.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\RS-championship3.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\RS-endurance1.xml"
  File "${BUILD_INST_DIR}\data\config\raceman\RS-endurance2.xml"

SectionEnd

Section -Drivers

  SetOverwrite on

  ; Customizable robots binaries
  SetOutPath "$INSTDIR\lib\drivers\axiom_ls2"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\axiom_ls2\*.*"

  SetOutPath "$INSTDIR\lib\drivers\shadow_ls2"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\shadow_ls2\*.*"

  SetOutPath "$INSTDIR\lib\drivers\usr_ls2"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\usr_ls2\*.*"

  SetOutPath "$INSTDIR\lib\drivers\axiom_rs"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\axiom_rs\*.*"

  SetOutPath "$INSTDIR\lib\drivers\shadow_rs"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\shadow_rs\*.*"

  SetOutPath "$INSTDIR\lib\drivers\usr_rs"
  File /r /x *.lib "${BUILD_INST_DIR}\lib\drivers\usr_rs\*.*"

  ; Customizable robot data files (car and track setups)
  SetOutPath "$INSTDIR\data\drivers\axiom_ls2"
  File /r "${BUILD_INST_DIR}\data\drivers\axiom_ls2\*.*"

  ; Robots data files (car and track setups)
  SetOutPath "$INSTDIR\data\drivers\shadow_ls2"
  File /r "${BUILD_INST_DIR}\data\drivers\shadow_ls2\*.*"

  SetOutPath "$INSTDIR\data\drivers\usr_ls2"
  File /r "${BUILD_INST_DIR}\data\drivers\usr_ls2\*.*"

  SetOutPath "$INSTDIR\data\drivers\axiom_rs"
  File /r "${BUILD_INST_DIR}\data\drivers\axiom_rs\*.*"

  SetOutPath "$INSTDIR\data\drivers\shadow_rs"
  File /r "${BUILD_INST_DIR}\data\drivers\shadow_rs\*.*"

  SetOutPath "$INSTDIR\data\drivers\usr_rs"
  File /r "${BUILD_INST_DIR}\data\drivers\usr_rs\*.*"

SectionEnd

Section -Cars

  SetOverwrite on
  
  ; Season 4 / LS2 & RS car models
  SetOutPath "$INSTDIR\data\cars\models\ls2-bavaria-g3gtr"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls2-bavaria-g3gtr\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls2-bavaria-nb"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls2-bavaria-nb\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls2-boxer-96rsr"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls2-boxer-96rsr\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls2-cavallo-360r"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls2-cavallo-360r\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls2-condor-c400r"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls2-condor-c400r\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls2-lynx-xkr"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls2-lynx-xkr\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls2-morley-r900"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls2-morley-r900\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls2-sprite-gts"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls2-sprite-gts\*.*"
  
    SetOutPath "$INSTDIR\data\cars\models\ls2-vortex-gt"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls2-vortex-gt\*.*"

  SetOutPath "$INSTDIR\data\cars\models\ls2-vulture-v6rs2"
  File /r "${BUILD_INST_DIR}\data\cars\models\ls2-vulture-v6rs2\*.*"

  SetOutPath "$INSTDIR\data\cars\models\rs1-citrus-cs4"
  File /r "${BUILD_INST_DIR}\data\cars\models\rs1-citrus-cs4\*.*"

  SetOutPath "$INSTDIR\data\cars\models\rs1-fmc-centre"
  File /r "${BUILD_INST_DIR}\data\cars\models\rs1-fmc-centre\*.*"

  SetOutPath "$INSTDIR\data\cars\models\rs1-kenji-08"
  File /r "${BUILD_INST_DIR}\data\cars\models\rs1-kenji-08\*.*"

  SetOutPath "$INSTDIR\data\cars\models\rs2-fai-dot"
  File /r "${BUILD_INST_DIR}\data\cars\models\rs2-fai-dot\*.*"

  SetOutPath "$INSTDIR\data\cars\models\rs2-lion-2m"
  File /r "${BUILD_INST_DIR}\data\cars\models\rs2-lion-2m\*.*"

SectionEnd

Section -Tracks

  SetOverwrite on
  
    ; Season 4 tracks
  SetOutPath "$INSTDIR\data\tracks\circuit\dijon"
  File /r "${BUILD_INST_DIR}\data\tracks\circuit\dijon\*.*"

  SetOutPath "$INSTDIR\data\tracks\circuit\hidden-valley"
  File /r "${BUILD_INST_DIR}\data\tracks\circuit\hidden-valley\*.*"

  SetOutPath "$INSTDIR\data\tracks\circuit\jarama"
  File /r "${BUILD_INST_DIR}\data\tracks\circuit\jarama\*.*"

  SetOutPath "$INSTDIR\data\tracks\circuit\migrants"
  File /r "${BUILD_INST_DIR}\data\tracks\circuit\migrants\*.*"

  SetOutPath "$INSTDIR\data\tracks\circuit\ruudskogen"
  File /r "${BUILD_INST_DIR}\data\tracks\circuit\ruudskogen\*.*"

  SetOutPath "$INSTDIR\data\tracks\dirt\brighton"
  File /r "${BUILD_INST_DIR}\data\tracks\dirt\brighton\*.*"

  SetOutPath "$INSTDIR\data\tracks\dirt\cagliari"
  File /r "${BUILD_INST_DIR}\data\tracks\dirt\cagliari\*.*"
    
  SetOutPath "$INSTDIR\data\tracks\dirt\casablanca"
  File /r "${BUILD_INST_DIR}\data\tracks\dirt\casablanca\*.*"

  SetOutPath "$INSTDIR\data\tracks\dirt\garguree"
  File /r "${BUILD_INST_DIR}\data\tracks\dirt\garguree\*.*"

  SetOutPath "$INSTDIR\data\tracks\dirt\ole-road-1"
  File /r "${BUILD_INST_DIR}\data\tracks\dirt\ole-road-1\*.*"
  
  SetOutPath "$INSTDIR\data\tracks\road\atlanta"
  File /r "${BUILD_INST_DIR}\data\tracks\road\atlanta\*.*"

  SetOutPath "$INSTDIR\data\tracks\road\longday"
  File /r "${BUILD_INST_DIR}\data\tracks\road\longday\*.*"

  SetOutPath "$INSTDIR\data\tracks\road\salty"
  File /r "${BUILD_INST_DIR}\data\tracks\road\salty\*.*"

SectionEnd

; This function is called at installer startup.
Function .onInit

  ; Check if the base package is installed and OK, and abort if not.
  !insertmacro CheckIfBasePackageInstalled

FunctionEnd

