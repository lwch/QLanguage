
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Calculator)

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

FILE(GLOB Lexer_Headers "../QLanguage/Lexer/*.h")
FILE(GLOB Lexer_Sources "../QLanguage/Lexer/*.cpp")
SET (Lexer ${Lexer_Headers} ${Lexer_Sources})
SOURCE_GROUP(Lexer FILES ${Lexer})

FILE(GLOB Parser_Headers "../QLanguage/Parser/*.h")
FILE(GLOB Parser_Sources "../QLanguage/Parser/*.cpp")
# remove parser for QLanguage
LIST(REMOVE_ITEM Parser_Headers "${CMAKE_CURRENT_SOURCE_DIR}/../QLanguage/Parser/Parser.h")
LIST(REMOVE_ITEM Parser_Sources "${CMAKE_CURRENT_SOURCE_DIR}/../QLanguage/Parser/Parser.cpp")
SET (Parser ${Parser_Headers} ${Parser_Sources})
SOURCE_GROUP(Parser FILES ${Parser})

FILE(GLOB Generator_Parser_Headers "Parser/*.h")
FILE(GLOB Generator_Parser_Sources "Parser/*.cpp")
SET (Generator_Parser ${Generator_Parser_Headers} ${Generator_Parser_Sources})
SOURCE_GROUP(Parser FILES ${Generator_Parser})

LIST (APPEND FILES
	${Lexer}
	${Parser}
	${Generator_Parser}
	Calculator.cpp
)

SET(CMAKE_CXX_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1 -D_DEBUG") 
SET(CMAKE_CXX_FLAGS_MINSIZEREL_INIT "/MT /O1 /Ob1 /D NDEBUG") 
SET(CMAKE_CXX_FLAGS_RELEASE_INIT "/MT /O2 /Ob2 /D NDEBUG") 
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /W4 /O2 /Ob1 /D NDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

ADD_EXECUTABLE(Calculator ${FILES})

SET_TARGET_PROPERTIES(Calculator PROPERTIES LINKER_LANGUAGE CXX)

TARGET_LINK_LIBRARIES(Calculator QCore)

IF (NOT TARGET QCore)
	SUBDIRS(../QCore)
ENDIF()
ADD_DEPENDENCIES(Calculator QCore)

ADD_CUSTOM_COMMAND(TARGET Calculator POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   "${CMAKE_CURRENT_SOURCE_DIR}/Calculator.ParserTable"
				   "${CMAKE_CURRENT_BINARY_DIR}/Calculator.ParserTable")