
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../check_bits.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../warning2error.cmake)

IF(NOT MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wl,-Map=QCoreUnitTest.map")
ENDIF()

PROJECT(QCoreUnitTest)

SET(DEBUG_LEVEL "3" CACHE STRING "1;2;3")
SET_PROPERTY(CACHE DEBUG_LEVEL PROPERTY STRINGS "1" "2" "3")

SET(ENABLE_SPEED_TEST OFF CACHE BOOL OFF)

ADD_DEFINITIONS(-DDEBUG_LEVEL=${DEBUG_LEVEL})

IF (ENABLE_SPEED_TEST)
    ADD_DEFINITIONS(-DTEST_SPEED)
ENDIF ()

FILE(GLOB Units_Headers "Units/*.h")
FILE(GLOB Units_Sources "Units/*.cpp")
SET (Units ${Units_Headers} ${Units_Sources})
SOURCE_GROUP(Units FILES ${Units})

LIST (APPEND FILES
    ${Units}
    QCoreUnitTest.cpp
)

SET(CMAKE_CXX_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1 -D_DEBUG") 
SET(CMAKE_CXX_FLAGS_MINSIZEREL_INIT "/MT /O1 /Ob1 /D NDEBUG") 
SET(CMAKE_CXX_FLAGS_RELEASE_INIT "/MT /O2 /Ob2 /D NDEBUG") 
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /W4 /O2 /Ob1 /D NDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

ADD_EXECUTABLE(QCoreUnitTest ${FILES})

TARGET_LINK_LIBRARIES(QCoreUnitTest QCore)

IF(WIN32)
    TARGET_LINK_LIBRARIES(QCoreUnitTest dbghelp psapi)
ENDIF(WIN32)

#SET_TARGET_PROPERTIES(QCoreUnitTest PROPERTIES COMPILE_FLAGS "-save-temps") 

IF (NOT TARGET QCore)
    SUBDIRS(../QCore)
ENDIF()

ADD_DEPENDENCIES(QCoreUnitTest QCore)
