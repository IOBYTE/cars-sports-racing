INCLUDE(../../../cmake/macros.cmake)

SET(ROBOT_SUBDIRS 0 1 2 3 4 5 6 7 8 9
                  01gpa-murasama 01gpa-hartbill-2002 01gpa-furia
                  01gpa-team-lynx 01gpa-ffr)

ROBOT_DATA(NAME axiom_01GPA
           FILES axiom_01GPA.xml default.xml readme.txt
           SUBDIRS ${ROBOT_SUBDIRS} PATTERNS *.xml *.rgb *.png *.jpg *.csv)
