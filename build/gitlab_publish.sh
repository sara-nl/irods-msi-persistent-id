#!/bin/bash
set -x
set -e

IRODS_VERSION=$( echo $VERSION | sed 's/^[^_]*_//' | sed 's/_/./g' )
OS_VERSION=$( echo $VERSION | cut -d '_' -f 1 )

if [ -z "$TEST_YUM_REPO" ]
then
    TEST_YUM_REPO=DMS-RPM-Testing
fi
if [ -z "$PROD_YUM_REPO" ]
then
    PROD_YUM_REPO=DMS-RPM-Production
fi

if [ -z "$CI_COMMIT_TAG" ]
then
    RELEASE=0
    BRANCH=$CI_COMMIT_REF_NAME
    PACK_VERSION=$CI_PIPELINE_ID
    REPOS=($TEST_YUM_REPO)
else
    RELEASE=0
    BRANCH="release"
    PACK_VERSION=$( echo $CI_COMMIT_TAG | sed 's/^v//g' )
    REPOS=($TEST_YUM_REPO $PROD_YUM_REPO)
fi

if [ "$OS_VERSION" = "centos7" ]
then
    TARGET_DIR=/data/RPMS/$CI_PROJECT_NAMESPACE/$BRANCH/CentOS/7/irods-$IRODS_VERSION
    if [ "$BRANCH" = "release" ] & [ "$CI_PROJECT_NAMESPACE" = "data-management-services" -o "$CI_PROJECT_NAMESPACE" = "B2SAFE" ]
    then
        REMOTE_TARGET_DIR=Centos/7/irods-$IRODS_VERSION/release/x86_64/Packages
    else
        REMOTE_TARGET_DIR=Centos/7/irods-$IRODS_VERSION/$BRANCH/x84_64/Packages
    fi
else
    echo "invalid os version $OS_VERSION"
    exit 1
fi
RPM=${PACKAGE_NAME}-${PACK_VERSION}-1.el7.x86_64.rpm


set +x
ret=0
for REPO in ${REPOS[@]}; do
    echo $REPO
done

for REPO in ${REPOS[@]}; do
    if [ -z "$ARTIE_KEY" ]
    then
        echo "no ARTIE_KEY defined: not published"
        echo curl -H "X-JFrog-Art-Api:$ARTIE_KEY" -XPUT https://artie.ia.surfsara.nl/artifactory/${REPO}/${REMOTE_TARGET_DIR}/${RPM} -T ${RPM}
        ret=1
    else
        curl -H "X-JFrog-Art-Api:$ARTIE_KEY" -XPUT https://artie.ia.surfsara.nl/artifactory/${REPO}/${REMOTE_TARGET_DIR}/${RPM} -T ${RPM}
    fi
done


exit $ret
