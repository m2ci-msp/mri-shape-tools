cmake_minimum_required(VERSION 2.7)
PROJECT(extract-slice)

SET(CMAKE_BUILD_TYPE release)
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -std=c++11 -march=native -Wall -Wextra -pedantic")
SET(CMAKE_C_FLAGS_RELEASE "-O2 -std=c++11 -march=native -Wall -Wextra -pedantic")
INCLUDE(../../cmake/ConfigureARMADILLO.cmake)
INCLUDE(../../cmake/ConfigureJSONCPP.cmake)

find_package(PkgConfig)
pkg_check_modules(CAIROMM REQUIRED cairomm-1.0)

IF(ARMADILLO_FOUND AND JSONCPP_FOUND)
  SET(SRC_FILE "../src/bin/main.cpp")

  INCLUDE_DIRECTORIES("../src/include")
  INCLUDE_DIRECTORIES("../../shared_include")
  INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE_DIR})
  INCLUDE_DIRECTORIES(${JSONCPP_INCLUDE_DIR})
  INCLUDE_DIRECTORIES(${CAIROMM_INCLUDE_DIRS})

  ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_FILE})
  TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${ARMADILLO_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    ${CAIROMM_LIBRARIES}
    )

  INSTALL (TARGETS ${PROJECT_NAME} DESTINATION bin)

ELSE(ARMADILLO_FOUND AND JSONCPP_FOUND)
  Message("PROBLEM: One of the required libraries not found. ${PROJECT_NAME} will not be compiled.")
ENDIF(ARMADILLO_FOUND AND JSONCPP_FOUND)
