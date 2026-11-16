INCLUDE(../../../cmake/macros.cmake)

SET(ROBOT_SUBDIRS 1 2 3 4 5 6
                  sc-spirit-300 sc-lynx-220 sc-vieringe-4s)

ROBOT_DATA(NAME axiom_sca
           FILES axiom_sca.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml *.rgb *.png *.jpg *.csv)
