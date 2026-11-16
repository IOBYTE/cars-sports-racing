INCLUDE(../../../cmake/macros.cmake)

# The data for the SCC clone of the ubiquitous DANDROID robot module.

SET(ROBOT_SUBDIRS 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17
                  sc-deckard-conejo sc-kanagawa-z35gts sc-murasama-nsx)

ROBOT_DATA(NAME axiom_scc
           FILES axiom_scc.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml *.rgb *.png *.jpg)
