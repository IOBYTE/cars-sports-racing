INCLUDE(../../../cmake/macros.cmake)

# The data for the 09GP5 clone of the ubiquitous axiom robot module.

SET(ROBOT_SUBDIRS 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
				  09gp5-fmc)

ROBOT_DATA(NAME axiom_09GP5
           FILES axiom_09GP5.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml logo.rgb *.png *.jpg *.csv)
