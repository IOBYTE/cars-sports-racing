INCLUDE(../../../cmake/macros.cmake)

SET(ROBOT_SUBDIRS 0 1 2 3 4 5
                  lp2-bravery-brc6 lp2-lolita-exr57
				  lp2-spire-s101)

ROBOT_DATA(NAME axiom_lp2
           FILES axiom_lp2.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml *.rgb *.png *.jpg *.csv)
