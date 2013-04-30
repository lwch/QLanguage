#!/bin/sh
DATE=`date '+%Y-%m-%d %H:%M:%S'`
MAILADDR="lwch748@gmail.com"
CURRENT_PATH=/opt/service
svn up ~/QLanguage
cmake ~/QLanguage/Source -G "Unix Makefiles" > $CURRENT_PATH/cmake.txt
cd ~/QLanguage/Source && make 2> $CURRENT_PATH/make.txt
cd ~/QLanguage/Source/QCoreUnitTest && ./QCoreUnitTest < $CURRENT_PATH/input.txt > $CURRENT_PATH/unittest.txt
mail -s "QLanguageUpdate on time $DATE(CMake Report)" $MAILADDR < $CURRENT_PATH/cmake.txt
mail -s "QLanguageUpdate on time $DATE(Make Report)" $MAILADDR < $CURRENT_PATH/make.txt
mail -s "QLanguageUpdate on time $DATE(UnitTest Report)" $MAILADDR < $CURRENT_PATH/unittest.txt
