#!/bin/sh

EXPORT_PATH=~/QLanguage/QLanguage
SOURCE_PATH="http://qlanguage.googlecode.com/svn/"
CMAKE_PATH=$EXPORT_PATH/./trunk/Source

if [ -d "$EXPORT_PATH" ]; then
  rm -fr $EXPORT_PATH
fi

svn export $SOURCE_PATH $EXPORT_PATH
cd $CMAKE_PATH && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cd $CMAKE_PATH && make
