cmake_minimum_required(VERSION 2.7)
PROJECT(weights-to-ema-json)

SET(CMAKE_BUILD_TYPE release)
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -std=c++11 -march=native -Wall -Wextra -pedantic")
SET(CMAKE_C_FLAGS_RELEASE "-O2 -std=c++11 -march=native -Wall -Wextra -pedantic")
INCLUDE(../../../cmake/ConfigureARMADILLO.cmake)
INCLUDE(../../../cmake/ConfigureJSONCPP.cmake)
INCLUDE(../../../cmake/ConfigureYAMLCPP.cmake)


IF(JSONCPP_FOUND AND ARMADILLO_FOUND AND YAMLCPP_FOUND)
  SET(SRC_FILE "../src/bin/main.cpp")

  INCLUDE_DIRECTORIES("../src/include")
  INCLUDE_DIRECTORIES("../../../shared_include")
  INCLUDE_DIRECTORIES(${JSONCPP_INCLUDE_DIR})
  INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE_DIR})
  INCLUDE_DIRECTORIES(${YAMLCPP_INCLUDE_DIR})

  ADD_EXECUTABLE(weights-to-ema-json ${SRC_FILE})
  TARGET_LINK_LIBRARIES(weights-to-ema-json
    ${JSONCPP_LIBRARIES}
    ${ARMADILLO_LIBRARIES}
    ${YAMLCPP_LIBRARIES}
    )

  INSTALL (TARGETS weights-to-ema-json DESTINATION bin)
ELSE(JSONCPP_FOUND AND ARMADILLO_FOUND AND YAMLCPP_FOUND)
  Message("PROBLEM: One of the required libraries not found. ${PROJECT_NAME} will not be compiled.")
ENDIF(JSONCPP_FOUND AND ARMADILLO_FOUND AND YAMLCPP_FOUND)
