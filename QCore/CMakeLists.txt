
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(QCore)

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
ADD_DEFINITIONS(-fpermissive)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")

#FILE(GLOB Combinator_Headers "Combinator/*.h")
#FILE(GLOB Combinator_Sources "Combinator/*.cpp")
#SET (Combinator ${Combinator_Headers} ${Combinator_Sources})
#SOURCE_GROUP(Combinator FILES ${Combinator})

#FILE(GLOB Global_Headers "Global/*.h")
#FILE(GLOB Global_Sources "Global/*.cpp")
#SET (Global ${Global_Headers} ${Global_Sources})
#SOURCE_GROUP(Global FILES ${Global})

#FILE(GLOB Lexer_Headers "Lexer/*.h")
#FILE(GLOB Lexer_Sources "Lexer/*.cpp")
#SET (Lexer ${Lexer_Headers} ${Lexer_Sources})
#SOURCE_GROUP(Lexer FILES ${Lexer})

#FILE(GLOB Tools_Headers "Tools/*.h")
#FILE(GLOB Tools_Sources "Tools/*.cpp")
#SET (Tools ${Tools_Headers} ${Tools_Sources})
#SOURCE_GROUP(Tools FILES ${Tools})

FILE(GLOB Library_Headers "Library/*.h")
FILE(GLOB Library_Sources "Library/*.cpp")
SET (Library ${Library_Headers} ${Library_Sources})
SOURCE_GROUP(Library FILES ${Library})

SET (FILES
#    ${Combinator}
#	${Global}
#	${Lexer}
#	${Tools}
	${Library}
	QCore.cpp
)

SET(CMAKE_C_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1") 
SET(CMAKE_C_FLAGS_MINSIZEREL_INIT "/MT /O1 /Ob1 /D NDEBUG") 
SET(CMAKE_C_FLAGS_RELEASE_INIT "/MT /O2 /Ob2 /D NDEBUG") 
SET(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /W4 /O2 /Ob1 /D NDEBUG")

ADD_LIBRARY(QCore STATIC ${FILES})
SET_TARGET_PROPERTIES(QCore PROPERTIES LINKER_LANGUAGE CXX)
