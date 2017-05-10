#
# Finds the relevant include directory and libraries used to develop
# with yaml-cpp

FIND_PATH(YAMLCPP_INCLUDE_DIR yaml-cpp/yaml.h
  ~/usr/include/
  /usr/local/include
  /usr/include
  /usr/include/
  /opt/local/include
 )

FIND_LIBRARY(YAMLCPP_LIBRARIES
  NAMES
   yaml-cpp
  PATHS
  ~/usr/lib/
   /usr/local/lib
   /usr/lib
   /opt/local/lib
)

SET(YAMLCPP_FOUND 0)
IF(YAMLCPP_INCLUDE_DIR)
  IF(YAMLCPP_LIBRARIES)
    SET(YAMLCPP_FOUND 1)
  ENDIF(YAMLCPP_LIBRARIES)
ENDIF(YAMLCPP_INCLUDE_DIR)

IF(YAMLCPP_FOUND)
  INCLUDE_DIRECTORIES(${YAMLCPP_INCLUDE_DIR})
ELSE(YAMLCPP_FOUND)
  MESSAGE("PROBLEM: YAMLCPP not found.")
ENDIF(YAMLCPP_FOUND)

MARK_AS_ADVANCED(YAMLCPP_INCLUDE_DIR
  YAMLCPP_LIBRARIES)
