INCLUDE(../../../cmake/macros.cmake)

SET(ROBOT_SUBDIRS 0 1 2 3 4 5 6 7 8 9 10 11
                  67gp-cavallo-123 67gp-guthrie-eagle
				  67gp-kuiper-t81 67gp-mango-ms7
				  67gp-murasama-37a 67gp-upping-ut19)

ROBOT_DATA(NAME axiom_67GP
           FILES axiom_67GP.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml *.rgb *.png *.jpg)
