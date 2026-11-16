INCLUDE(../../../cmake/macros.cmake)

# The data for the MPA12 clone of the ubiquitous dandroid robot module.

SET(ROBOT_SUBDIRS 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17
				  11gpa-murasama)

ROBOT_DATA(NAME axiom_11GPA
           FILES axiom_11GPA.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml *.rgb *.png *.jpg *.csv)
