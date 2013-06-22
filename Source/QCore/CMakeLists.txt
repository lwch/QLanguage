
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(QCore)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../check_bits.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../warning2error.cmake)

SET(DEBUG_LEVEL "3" CACHE STRING "1;2;3")

ADD_DEFINITIONS(-DDEBUG_LEVEL=${DEBUG_LEVEL})

# Common Library
FILE(GLOB Library_Headers "Library/*.h")
FILE(GLOB Library_Sources "Library/*.cpp")
SET (Library ${Library_Headers} ${Library_Sources})
SOURCE_GROUP(Library FILES ${Library})

# Regex Library
FILE(GLOB Library_Regex_Headers "Library/regex/*.h")
FILE(GLOB Library_Regex_Sources "Library/regex/*.cpp")
SET (Library_Regex ${Library_Regex_Headers} ${Library_Regex_Sources})
SOURCE_GROUP(Library\\regex FILES ${Library_Regex})

# Combinator Library
FILE(GLOB Library_Combinator_Headers "Library/combinator/*.h")
FILE(GLOB Library_Combinator_Sources "Library/combinator/*.cpp")
SET (Library_Combinator ${Library_Combinator_Headers} ${Library_Combinator_Sources})
SOURCE_GROUP(Library\\combinator FILES ${Library_Combinator})

# Graph Library
FILE(GLOB Library_Graph_BMP_Headers "Library/graph/bmp/*.h")
FILE(GLOB Library_Graph_BMP_Sources "Library/graph/bmp/*.cpp")
SET (Library_Graph_BMP ${Library_Graph_BMP_Headers} ${Library_Graph_BMP_Sources})
SOURCE_GROUP(Library\\graph\\bmp FILES ${Library_Graph_BMP})

FILE(GLOB Library_Graph_Headers "Library/graph/*.h")
FILE(GLOB Library_Graph_Sources "Library/graph/*.cpp")
SET (Library_Graph ${Library_Graph_Headers} ${Library_Graph_Sources})
SOURCE_GROUP(Library\\graph FILES ${Library_Graph})

LIST (APPEND Library_Graph
	${Library_Graph_BMP}
)

# Files
SET (FILES
	${Library}
	${Library_Regex}
	${Library_Combinator}
	${Library_Graph}
	QCore.cpp
)

SET(CMAKE_C_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1") 
SET(CMAKE_C_FLAGS_MINSIZEREL_INIT "/MT /O1 /Ob1 /D NDEBUG") 
SET(CMAKE_C_FLAGS_RELEASE_INIT "/MT /O2 /Ob2 /D NDEBUG") 
SET(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /W4 /O2 /Ob1 /D NDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

ADD_LIBRARY(QCore STATIC ${FILES})
SET_TARGET_PROPERTIES(QCore PROPERTIES LINKER_LANGUAGE CXX)