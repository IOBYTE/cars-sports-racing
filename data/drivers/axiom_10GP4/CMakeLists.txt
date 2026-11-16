INCLUDE(../../../cmake/macros.cmake)

# The data for the 09GP5 clone of the ubiquitous axiom robot module.

SET(ROBOT_SUBDIRS 0 1 2 3 
				  10gp4-diamond)

ROBOT_DATA(NAME axiom_10GP4
           FILES axiom_10GP4.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml logo.rgb *.png *.jpg)
