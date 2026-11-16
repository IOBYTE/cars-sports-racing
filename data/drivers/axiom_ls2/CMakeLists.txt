INCLUDE(../../../cmake/macros.cmake)

SET(ROBOT_SUBDIRS 0 1 2 3 4 5 6 7 8 9
                  ls2-condor-c400r ls2-lynx-xkr ls2-sprite-gts
                  ls2-vortex-gt ls2-vulture-v6rs2)

ROBOT_DATA(NAME axiom_ls2
           FILES axiom_ls2.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml *.rgb *.png *.jpg *.csv)
