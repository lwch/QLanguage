#/bin/sh

if [ x$1 != x ]; then
  if [ $1 = "i386" ] || [ $1 = "amd64" ]; then
    if [ $1 = "i386" ]; then
      PLATFORM=x86
    else
      PLATFORM=x64
    fi
    CURRENT_PATH=~/rpmbuild/SPECS/
    SOURCE_PATH=~/QLanguage_GitHub/
    DEST_PATH=QLanguage/
    COMPRESS_PATH=$CURRENT_PATH/QLanguage.tar.gz # always change .spec
    RPMBUILD_COMPRESS_PATH=~/rpmbuild/SOURCES/QLanguage.tar.gz
    SPEC_PATH=$CURRENT_PATH/QLanguage.$PLATFORM.spec

    cd $CURRENT_PATH
    if [ -d "$DEST_PATH" ]; then
      rm -fr $DEST_PATH
    fi

    if [ -f "$SPEC_PATH" ]; then
      rm -f $SPEC_PATH
    fi

    cd $SOURCE_PATH
    git svn rebase
    cd $CURRENT_PATH
    cp -R $SOURCE_PATH $DEST_PATH
    rm -fr $DEST_PATH/.git/ # for git
    tar -zcf $COMPRESS_PATH $DEST_PATH
    mv -f $COMPRESS_PATH $RPMBUILD_COMPRESS_PATH
    sed "s/os/$PLATFORM/" QLanguage.spec > $SPEC_PATH
    rpmbuild -bb $SPEC_PATH
  else
    echo "Unknow archiecture type(i386 or amd64)"
  fi
else
  echo "Please input architecture(i386 or amd64)"
fi
