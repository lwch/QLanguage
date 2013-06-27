#!/bin/sh

if [ x$1 != x ]; then
  if [ $1 = "i386" ] || [ $1 = "amd64" ]; then
    DEB_PATH=~/QLanguage_Deb_$1/
    BINARY_PATH=~/QLanguage/Source/QLanguage/QLanguage

    # make debian path
    if [ -d "$DEB_PATH" ]; then
      rm -fr $DEB_PATH
    fi
    mkdir -p $DEB_PATH/./DEBIAN/
    mkdir -p $DEB_PATH/./usr/local/bin/
    #cp control $DEB_PATH/./DEBIAN/
    cp $BINARY_PATH $DEB_PATH/./usr/local/bin/

    # change archiecture
    sed "s/os/$1/" control > $DEB_PATH/./DEBIAN/control

    dpkg -b $DEB_PATH
  else
    echo "Unknow archiecture type(i386 or amd64)"
  fi
else
  echo "Please input architecture(i386 or amd64)"
fi
