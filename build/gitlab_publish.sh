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
    PACK_VERSION=$( echo $CI_COMMIT_TAG | sed 's/^v//g' )
fi

if [ "$OS_VERSION" = "centos7" ]
then
    TARGET_DIR=/data/RPMS/$CI_PROJECT_NAMESPACE/$BRANCH/CentOS/7/irods-$IRODS_VERSION
    if [ "$BRANCH" = "release" ] & [ "$CI_PROJECT_NAMESPACE" = "data-management-services" ]
    then
        REMOTE_TARGET_DIR=/repos/CentOS/7/irods-$IRODS_VERSION/Packages
    else
        REMOTE_TARGET_DIR=/repos/CentOS/7/$CI_PROJECT_NAMESPACE/$BRANCH/irods-$IRODS_VERSION/Packages
    fi
else
    echo "invalid os version $OS_VERSION"
    exit 1
fi
RPM=${PACKAGE_NAME}-${PACK_VERSION}-1.el7.x86_64.rpm


ssh -oStrictHostKeyChecking=no $YUM_SERVER mkdir -p $REMOTE_TARGET_DIR
scp -oStrictHostKeyChecking=no $TARGET_DIR/x86_64/$RPM $YUM_SERVER:$REMOTE_TARGET_DIR/$RPM
ssh -oStrictHostKeyChecking=no $YUM_SERVER createrepo --update $REMOTE_TARGET_DIR
