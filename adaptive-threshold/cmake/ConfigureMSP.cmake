FIND_PATH(MSP_INCLUDE_DIR flags/Flag.h
   ~/usr/include/msp-tonguemodel
   /usr/local/include/msp-tonguemodel
   /usr/include/msp-tonguemodel
)

SET(MSP_FOUND 0)
IF(MSP_INCLUDE_DIR)
  SET(MSP_FOUND 1)
ENDIF(MSP_INCLUDE_DIR)

IF(MSP_FOUND)
  INCLUDE_DIRECTORIES(${MSP_INCLUDE_DIR})
ELSE(MSP_FOUND)
  MESSAGE("PROBLEM: MSP not found.")
ENDIF(MSP_FOUND)