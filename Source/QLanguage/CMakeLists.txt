
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(QLanguage)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../check_bits.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../warning2error.cmake)

IF(NOT MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wl,-Map=QLanguage.map")
ENDIF()

SET(DEBUG_LEVEL "3" CACHE STRING "1;2;3")
SET_PROPERTY(CACHE DEBUG_LEVEL PROPERTY STRINGS "1" "2" "3")

ADD_DEFINITIONS(-DDEBUG_LEVEL=${DEBUG_LEVEL})

FILE(GLOB Lexer_Headers "Lexer/*.h")
FILE(GLOB Lexer_Sources "Lexer/*.cpp")
SET (Lexer ${Lexer_Headers} ${Lexer_Sources})
SOURCE_GROUP(Lexer FILES ${Lexer})

FILE(GLOB Parser_Headers "Parser/*.h")
FILE(GLOB Parser_Sources "Parser/*.cpp")
SET (Parser ${Parser_Headers} ${Parser_Sources})
SOURCE_GROUP(Parser FILES ${Parser})

FILE(GLOB Parser_SyntaxTree_Headers "Parser/SyntaxTree/*.h")
FILE(GLOB Parser_SyntaxTree_Sources "Parser/SyntaxTree/*.cpp")
SET (Parser_SyntaxTree ${Parser_SyntaxTree_Headers} ${Parser_SyntaxTree_Sources})
SOURCE_GROUP(Parser\\SyntaxTree FILES ${Parser_SyntaxTree})

FILE(GLOB VirtualMachine_Headers "VirtualMachine/*.h")
FILE(GLOB VirtualMachine_Sources "VirtualMachine/*.cpp")
SET (VirtualMachine ${VirtualMachine_Headers} ${VirtualMachine_Sources})
SOURCE_GROUP(VirtualMachine FILES ${VirtualMachine})

SET (FILES
     ${Lexer}
     ${Parser}
     ${Parser_SyntaxTree}
     ${VirtualMachine}
     QLanguage.cpp
)

SET(CMAKE_CXX_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1 -D_DEBUG") 
SET(CMAKE_CXX_FLAGS_MINSIZEREL_INIT "/MT /O1 /Ob1 /D NDEBUG") 
SET(CMAKE_CXX_FLAGS_RELEASE_INIT "/MT /O2 /Ob2 /D NDEBUG") 
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /W4 /O2 /Ob1 /D NDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

ADD_EXECUTABLE(QLanguage ${FILES})

IF(NOT WIN32)
    IF(X86)
        INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/QLanguage.x86.ParserTable" DESTINATION bin)
    ELSE(X86)
        INSTALL(FILES QLanguage.x64.ParserTable DESTINATION bin)
    ENDIF(X86)
    INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/QLanguage DESTINATION bin)
ENDIF(NOT WIN32)

TARGET_LINK_LIBRARIES(QLanguage QCore)

IF(WIN32)
    TARGET_LINK_LIBRARIES(QLanguage dbghelp psapi)
ENDIF(WIN32)

IF (NOT TARGET QCore)
	SUBDIRS(../QCore)
ENDIF()
ADD_DEPENDENCIES(QLanguage QCore)

IF(X86)
    ADD_CUSTOM_COMMAND(TARGET QLanguage PRE_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${CMAKE_CURRENT_SOURCE_DIR}/QLanguage.x86.ParserTable"
                       "${CMAKE_CURRENT_BINARY_DIR}/QLanguage.x86.ParserTable")
ELSE(X86)
    ADD_CUSTOM_COMMAND(TARGET QLanguage PRE_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${CMAKE_CURRENT_SOURCE_DIR}/QLanguage.x64.ParserTable"
                       "${CMAKE_CURRENT_BINARY_DIR}/QLanguage.x64.ParserTable")
ENDIF(X86)
