
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(QLanguage)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../check_bits.cmake)

SET(DEBUG_LEVEL "3" CACHE STRING "1;2;3")

IF(NOT MSVC)
	SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	ADD_DEFINITIONS(-DGNUC)
ELSE()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-DMSVC)
ENDIF(NOT MSVC)

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

LIST (APPEND FILES
	${Lexer}
	${Parser}
	${Parser_SyntaxTree}
	QLanguage.cpp
)

SET(CMAKE_CXX_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1 -D_DEBUG") 
SET(CMAKE_CXX_FLAGS_MINSIZEREL_INIT "/MT /O1 /Ob1 /D NDEBUG") 
SET(CMAKE_CXX_FLAGS_RELEASE_INIT "/MT /O2 /Ob2 /D NDEBUG") 
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /W4 /O2 /Ob1 /D NDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

ADD_EXECUTABLE(QLanguage ${FILES})

IF(NOT WIN32)
	INSTALL(PROGRAMS QLanguage DESTINATION bin)
ENDIF(NOT WIN32)

SET_TARGET_PROPERTIES(QLanguage PROPERTIES LINKER_LANGUAGE CXX)

TARGET_LINK_LIBRARIES(QLanguage QCore)

IF (NOT TARGET QCore)
	SUBDIRS(../QCore)
ENDIF()
ADD_DEPENDENCIES(QLanguage QCore)

IF(X86)
    ADD_CUSTOM_COMMAND(TARGET QLanguage POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${CMAKE_CURRENT_SOURCE_DIR}/QLanguage.x86.ParserTable"
                       "${CMAKE_CURRENT_BINARY_DIR}/QLanguage.x86.ParserTable")
ELSE(X86)
    ADD_CUSTOM_COMMAND(TARGET QLanguage POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${CMAKE_CURRENT_SOURCE_DIR}/QLanguage.x64.ParserTable"
                       "${CMAKE_CURRENT_BINARY_DIR}/QLanguage.x64.ParserTable")
ENDIF(X86)
