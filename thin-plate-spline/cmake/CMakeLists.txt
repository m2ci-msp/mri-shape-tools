cmake_minimum_required(VERSION 2.7)
PROJECT(thin-plate-spline)

SET(CMAKE_BUILD_TYPE release)
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -std=c++11 -march=native -Wall -Wextra -pedantic")
SET(CMAKE_C_FLAGS_RELEASE "-O2 -std=c++11 -march=native -Wall -Wextra -pedantic")
INCLUDE(../../cmake/ConfigureARMADILLO.cmake)

IF(ARMADILLO_FOUND)
  SET(SRC_FILE "../src/bin/main.cpp")

  INCLUDE_DIRECTORIES("../src/include")
  INCLUDE_DIRECTORIES("../../shared_include")
  INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE_DIR})

  ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_FILE})
  TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${ARMADILLO_LIBRARIES}
    )

  INSTALL (TARGETS ${PROJECT_NAME} DESTINATION bin)

ELSE(ARMADILLO_FOUND)
  Message("PROBLEM: One of the required libraries not found. ${PROJECT_NAME} will not be compiled.")
ENDIF(ARMADILLO_FOUND)
