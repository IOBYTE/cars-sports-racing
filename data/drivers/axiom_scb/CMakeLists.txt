INCLUDE(../../../cmake/macros.cmake)

SET(ROBOT_SUBDIRS 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17
                  sc-boxer-96 sc-cavallo-360 sc-fmc-gt4)

ROBOT_DATA(NAME axiom_scb
           FILES axiom_scb.xml default.xml logo.rgb readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml logo.rgb *.png *.jpg)
