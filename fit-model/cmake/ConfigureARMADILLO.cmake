FIND_PATH(ARMADILLO_INCLUDE_DIR armadillo
   /usr/local/include
   /usr/include
)

FIND_LIBRARY(ARMADILLO_LIBRARY
  NAMES
   armadillo
  PATHS
   /usr/local/lib
   /usr/lib
)

SET(ARMADILLO_FOUND 0)
IF(ARMADILLO_INCLUDE_DIR)
  IF(ARMADILLO_LIBRARY)
    SET(ARMADILLO_FOUND 1)
  ENDIF(ARMADILLO_LIBRARY)
ENDIF(ARMADILLO_INCLUDE_DIR)

IF(ARMADILLO_FOUND)
  INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE_DIR})
ELSE(ARMADILLO_FOUND)
  MESSAGE("PROBLEM: ARMADILLO not found.")
ENDIF(ARMADILLO_FOUND)

SET(ARMADILLO_LIBRARIES ${ARMADILLO_LIBRARY} )