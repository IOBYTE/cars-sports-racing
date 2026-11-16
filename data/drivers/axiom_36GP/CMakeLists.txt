INCLUDE(../../../cmake/macros.cmake)

SET(ROBOT_SUBDIRS 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
                  36gp-century-r11b 36gp-ettore-t59
                  36gp-milano-12c36 36gp-milano-tipop3 
				  36gp-motorunion-typec 36gp-silber-w25b
				  36gp-tridenti-6c34 36gp-tridenti-v8ri)

ROBOT_DATA(NAME axiom_36GP
           FILES axiom_36GP.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml *.rgb *.png *.jpg)
