BRANCH=$1

if [ -z "$BRANCH" ]
then
    ARG_BRANCH=""
    SUFFIX_BRANCH=""
else
    ARG_BRANCH=
    SUFFIX_BRANCH="-$BRANCH"
fi

set -x 
tar --transform 's/^\./msi-persistent-id'${SUFFIX_BRANCH}'-1.0/' -cvzf /root/rpmbuild/SOURCES/msi-persistent-id${SUFFIX_BRANCH}-1.0.tar.gz .
if [ -z "$BRANCH" ]
then
    rpmbuild -bb msi-persistent-id.spec
else
    rpmbuild -bb --define "branch $BRANCH" msi-persistent-id.spec
fi
