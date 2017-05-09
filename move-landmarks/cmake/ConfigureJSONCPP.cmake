#
# Just finds the relevant include directory and libraries used to develop
# with jsoncpp

FIND_PATH(JSONCPP_INCLUDE_DIR json/json.h
  ~/usr/include/
  /usr/local/include
  /usr/include
  /usr/include/jsoncpp
  /opt/local/include
 )

FIND_LIBRARY(JSONCPP_LIBRARIES
  NAMES
   jsoncpp
  PATHS
  ~/usr/lib/
   /usr/local/lib
   /usr/lib
   /opt/local/lib
)

SET(JSONCPP_FOUND 0)
IF(JSONCPP_INCLUDE_DIR)
  IF(JSONCPP_LIBRARIES)
    SET(JSONCPP_FOUND 1)
  ENDIF(JSONCPP_LIBRARIES)
ENDIF(JSONCPP_INCLUDE_DIR)

IF(JSONCPP_FOUND)
  INCLUDE_DIRECTORIES(${JSONCPP_INCLUDE_DIR})
ELSE(JSONCPP_FOUND)
  MESSAGE("PROBLEM: JSONCPP not found.")
ENDIF(JSONCPP_FOUND)

MARK_AS_ADVANCED(JSONCPP_INCLUDE_DIR
  JSONCPP_LIBRARIES)
