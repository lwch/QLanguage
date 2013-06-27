#/bin/sh
CURRENT_PATH=~/rpmbuild/SPECS/
SOURCE_PATH=~/QLanguage_GitHub/
DEST_PATH=QLanguage/
COMPRESS_PATH=$CURRENT_PATH/QLanguage.tar.gz # always change .spec
RPMBUILD_COMPRESS_PATH=~/rpmbuild/SOURCES/QLanguage.tar.gz
SPEC_PATH=$CURRENT_PATH/QLanguage.spec

cd $CURRENT_PATH
if [ -d "$DEST_PATH" ]; then
  rm -fr $DEST_PATH
fi

cd $SOURCE_PATH
git svn rebase
cd $CURRENT_PATH
cp -R $SOURCE_PATH $DEST_PATH
rm -fr $DEST_PATH/.git/ # for git
tar -zcf $COMPRESS_PATH $DEST_PATH
mv -f $COMPRESS_PATH $RPMBUILD_COMPRESS_PATH
rpmbuild -bb $SPEC_PATH
