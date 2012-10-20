
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(QLanguage)

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
ADD_DEFINITIONS(-fpermissive)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
FILE(GLOB VLD_Headers "../QCore/vld/*.h")
FILE(GLOB VLD_Sources "../QCore/vld/*.cpp")
SET (VLD ${VLD_Headers} ${VLD_Sources})
SOURCE_GROUP(vld FILES ${VLD})
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
SET (FILES
    ${VLD}
)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
LIST (APPEND FILES
	QLanguage.cpp
)

SET(CMAKE_C_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1") 
SET(CMAKE_C_FLAGS_MINSIZEREL_INIT "/MT /O1 /Ob1 /D NDEBUG") 
SET(CMAKE_C_FLAGS_RELEASE_INIT "/MT /O2 /Ob2 /D NDEBUG") 
SET(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /W4 /O2 /Ob1 /D NDEBUG")

ADD_EXECUTABLE(QLanguage ${FILES})
IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
SET(VLD_Path ${PROJECT_SOURCE_DIR}/../QCore/vld)

# if target is win64
IF(${CMAKE_CL_64})
SET(VLD_Path ${VLD_Path}/Win64/)
ADD_CUSTOM_COMMAND(
                   TARGET QLanguage
                   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy ${VLD_Path}/vld_x64.dll ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/vld_x64.dll
                   COMMAND ${CMAKE_COMMAND} -E copy ${VLD_Path}/dbghelp.dll ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/dbghelp.dll
                   COMMAND ${CMAKE_COMMAND} -E copy ${VLD_Path}/Microsoft.DTfW.DHL.manifest ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Microsoft.DTfW.DHL.manifest
				   )
# target win32
ELSE(${CMAKE_CL_64})
SET(VLD_Path ${VLD_Path}/Win32/)
ADD_CUSTOM_COMMAND(
                   TARGET QLanguage
                   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy ${VLD_Path}/vld_x86.dll ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/vld_x86.dll
                   COMMAND ${CMAKE_COMMAND} -E copy ${VLD_Path}/dbghelp.dll ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/dbghelp.dll
                   COMMAND ${CMAKE_COMMAND} -E copy ${VLD_Path}/Microsoft.DTfW.DHL.manifest ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Microsoft.DTfW.DHL.manifest
				   )
ENDIF(${CMAKE_CL_64})

LINK_DIRECTORIES(${VLD_PATH})

TARGET_LINK_LIBRARIES(QLanguage ${VLD_Path}/vld.lib)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

TARGET_LINK_LIBRARIES(QLanguage QCore)

SUBDIRS(../QCore)
ADD_DEPENDENCIES(QLanguage QCore)
