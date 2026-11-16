INCLUDE(../../../cmake/macros.cmake)

SET(ROBOT_SUBDIRS 0 1 2 3 4 5 6 7 8 9
                  91gp-boxer-amani 91gp-cavallo-642 91gp-deckard-ags01
                  91gp-deckard-b190 91gp-diamond-fw14)

ROBOT_DATA(NAME axiom_91GP
           FILES axiom_91GP.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml *.rgb *.png *.jpg)
