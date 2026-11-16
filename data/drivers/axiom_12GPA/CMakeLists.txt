INCLUDE(../../../cmake/macros.cmake)

# The data for the 12GPA clone of the ubiquitous axiom robot module.

SET(ROBOT_SUBDIRS 0 1 2 3 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
				  12gpa-deckard 12gpa-murasama 12gpa-spirit)

ROBOT_DATA(NAME axiom_12GPA
           FILES axiom_12GPA.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml *.rgb *.png *.jpg)
