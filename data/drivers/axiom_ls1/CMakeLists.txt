INCLUDE(../../../cmake/macros.cmake)

# The data for the LS1 clone of the ubiquitous AXIOM robot module.

SET(ROBOT_SUBDIRS 0 1 2 3 4 5 6 7 8 9
                  ls1-cavallo-570s1 ls1-marisatech-gt4r ls1-taipan-ltsr
		  		  ls1-toro-rgt ls1-zentek-z7r)

ROBOT_DATA(NAME axiom_ls1
           FILES axiom_ls1.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml *.rgb *.png *.jpg)
