INCLUDE(../../../cmake/macros.cmake)

FILE(GLOB glf LIST_DIRECTORIES false *.glf)
FILE(GLOB ttf LIST_DIRECTORIES false *.ttf)

CSR_INSTALL_FILES(DATA data/fonts FILES ${glf} ${ttf})

FILE(GLOB 5by7 LIST_DIRECTORIES false 5by7/*.*)
FILE(GLOB ArmataHL LIST_DIRECTORIES false ArmataHL/*.*)
FILE(GLOB Montserrant LIST_DIRECTORIES false Montserrant/*.*)
FILE(GLOB vera LIST_DIRECTORIES false vera/*.*)

CSR_INSTALL_FILES(DATA data/fonts/5by7 FILES ${5by7})
CSR_INSTALL_FILES(DATA data/fonts/ArmataHL FILES ${ArmataHL})
CSR_INSTALL_FILES(DATA data/fonts/Montserrant FILES ${Montserrant})
CSR_INSTALL_FILES(DATA data/fonts/vera FILES ${vera})
