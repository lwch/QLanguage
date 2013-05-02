#!/bin/sh

DATE=`date '+%Y-%m-%d %H:%M:%S'`
MAILADDR="lwch748@gmail.com"
CURRENT_PATH=/opt/service
SOURCE_PATH="http://qlanguage.googlecode.com/svn/trunk/"
EXPORT_PATH=~/QLanguage/
CMAKE_PATH=$EXPORT_PATH/./Source
UNITTEST_PATH=$CMAKE_PATH/./QCoreUnitTest
UNITTEST_EXEC=QCoreUnitTest

if [ -d "$EXPORT_PATH" ]; then
  rm -fr $EXPORT_PATH
fi
svn export $SOURCE_PATH $EXPORT_PATH
cd $CMAKE_PATH && cmake -G "Unix Makefiles" > $CURRENT_PATH/cmake.txt
cd $CMAKE_PATH && make 2> $CURRENT_PATH/make.txt
$UNITTEST_PATH/$UNITTEST_EXEC < $CURRENT_PATH/input.txt > $CURRENT_PATH/unittest.txt
mail -s "QLanguageUpdate on time $DATE(CMake Report)" $MAILADDR < $CURRENT_PATH/cmake.txt
mail -s "QLanguageUpdate on time $DATE(Make Report)" $MAILADDR < $CURRENT_PATH/make.txt
mail -s "QLanguageUpdate on time $DATE(UnitTest Report)" $MAILADDR < $CURRENT_PATH/unittest.txt
