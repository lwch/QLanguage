
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(QCore)

SET(DEBUG_LEVEL 3 CACHE STRING "")
SET_PROPERTY(CACHE DEBUG_LEVEL PROPERTY STRINGS "1;2;3")

IF(NOT MSVC)
	SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
ELSE()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
ENDIF(NOT MSVC)

ADD_DEFINITIONS(-DDEBUG_LEVEL=${DEBUG_LEVEL})

FILE(GLOB Library_Headers "Library/*.h")
FILE(GLOB Library_Sources "Library/*.cpp")
SET (Library ${Library_Headers} ${Library_Sources})
SOURCE_GROUP(Library FILES ${Library})

FILE(GLOB Library_Regex_Headers "Library/regex/*.h")
FILE(GLOB Library_Regex_Sources "Library/regex/*.cpp")
SET (Library_Regex ${Library_Regex_Headers} ${Library_Regex_Sources})
SOURCE_GROUP(Library\\regex FILES ${Library_Regex})

FILE(GLOB Library_Combinator_Headers "Library/combinator/*.h")
FILE(GLOB Library_Combinator_Sources "Library/combinator/*.cpp")
SET (Library_Combinator ${Library_Combinator_Headers} ${Library_Combinator_Sources})
SOURCE_GROUP(Library\\combinator FILES ${Library_Combinator})

SET (FILES
	${Library}
	${Library_Regex}
	${Library_Combinator}
	QCore.cpp
)

SET(CMAKE_C_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1") 
SET(CMAKE_C_FLAGS_MINSIZEREL_INIT "/MT /O1 /Ob1 /D NDEBUG") 
SET(CMAKE_C_FLAGS_RELEASE_INIT "/MT /O2 /Ob2 /D NDEBUG") 
SET(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /W4 /O2 /Ob1 /D NDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

ADD_LIBRARY(QCore STATIC ${FILES})
SET_TARGET_PROPERTIES(QCore PROPERTIES LINKER_LANGUAGE CXX)
