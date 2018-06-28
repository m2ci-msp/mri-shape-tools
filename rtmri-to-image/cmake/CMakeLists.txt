cmake_minimum_required(VERSION 2.7)
PROJECT(rtmri-to-image)

SET(CMAKE_BUILD_TYPE release)
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -std=c++11 -march=native -Wall -Wextra -pedantic")
SET(CMAKE_C_FLAGS_RELEASE "-O2 -std=c++11 -march=native -Wall -Wextra -pedantic")
INCLUDE(../../cmake/ConfigureJSONCPP.cmake)
INCLUDE(../../cmake/ConfigureARMADILLO.cmake)

find_path( AVCODEC_INCLUDE_DIR libavcodec/avcodec.h )
find_library( AVCODEC_LIBRARY avcodec )
find_library( AVUTIL_LIBRARY avutil )
find_library( AVFORMAT_LIBRARY avformat )

IF(JSONCPP_FOUND AND ARMADILLO_FOUND)
  SET(SRC_FILE "../src/bin/main.cpp")

  INCLUDE_DIRECTORIES("../src/include")
  INCLUDE_DIRECTORIES("../../shared_include")
  INCLUDE_DIRECTORIES(${JSONCPP_INCLUDE_DIR})
  INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE_DIR})
  INCLUDE_DIRECTORIES(${AVCODEC_INCLUDE_DIR})

  ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_FILE})
  TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${JSONCPP_LIBRARIES}
    ${ARMADILLO_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${AVFORMAT_LIBRARIES}
    )

  INSTALL (TARGETS ${PROJECT_NAME} DESTINATION bin)

ELSE(JSONCPP_FOUND AND ARMADILLO_FOUND)
  Message("PROBLEM: One of the required libraries not found. ${PROJECT_NAME} will not be compiled.")
ENDIF(JSONCPP_FOUND AND ARMADILLO_FOUND)
