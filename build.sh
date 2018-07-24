#!/bin/bash
BRANCH=$1
VERSION=$2

cd $( dirname "$0" )
if [ -z "$BRANCH" ]
then
    SUFFIX_BRANCH=""
else
    SUFFIX_BRANCH="-$BRANCH"
fi

set -x 
tar --transform 's/^\./msi-persistent-id'${SUFFIX_BRANCH}'-'${VERSION}'/' -cvzf /root/rpmbuild/SOURCES/msi-persistent-id${SUFFIX_BRANCH}-${VERSION}.tar.gz .
echo ${SUFFIX_BRANCH}
echo "${ARG_BRANCH}"

if [ -z "$BRANCH" ]
then
    rpmbuild -bb --define "version $VERSION" msi-persistent-id.spec
else
   rpmbuild -bb --define "branch $BRANCH" --define "version $VERSION" msi-persistent-id.spec
fi

rsync -r /root/rpmbuild/RPMS/ /build/RPMS/
chmod -R a+rw  /build/RPMS
