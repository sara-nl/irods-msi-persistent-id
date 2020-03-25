#!/bin/bash
set -x
set -e

IRODS_VERSION=$( echo $VERSION | sed 's/^[^_]*_//' | sed 's/_/./g' )
OS_VERSION=$( echo $VERSION | cut -d '_' -f 1 )


if [ -z "$CI_COMMIT_TAG" ]
then
    BRANCH=$CI_COMMIT_REF_NAME
    PACK_VERSION=$CI_PIPELINE_ID
else
    BRANCH="release"
    PACK_VERSION=$CI_COMMIT_TAG
fi

if [ "$OS_VERSION" = "centos7" ]
then
    TARGET_DIR=/data/RPMS/$CI_PROJECT_NAMESPACE/$BRANCH/CentOS/7/irods-$IRODS_VERSION
else
    echo "invalid os version $OS_VERSION"
    exit 1
fi
mkdir -p $TARGET_DIR

docker-compose -f build/docker-compose.yml pull

docker-compose -f build/docker-compose.yml run icat sudo -u rpmbuild \
                   /home/rpmbuild/build_rpm.sh \
                   --spec-file /build/msi-persistent-id.spec \
                   --package $PACKAGE_NAME \
                   --version $PACK_VERSION

docker cp build_icat_run_1:/home/rpmbuild/rpmbuild/RPMS/x86_64 $TARGET_DIR


