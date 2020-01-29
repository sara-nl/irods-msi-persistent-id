#!/bin/bash
export VOLUME=..

if [ -z "$1" ]
then
    export VERSION=centos7_4_2_6
else
    export VERSION=$1
fi

if [ -z "$2" ]
then
    export PACK_VERSION=1.0
else
    export PACK_VERSION=$1
fi

docker-compose -f docker-compose-local.yml run icat sudo -u rpmbuild \
                   /home/rpmbuild/build_rpm.sh \
                   --spec-file /build/msi-persistent-id.spec \
                   --package msi-persistent-id \
                   --version $PACK_VERSION

docker cp build_icat_run_1:/home/rpmbuild/rpmbuild/RPMS/x86_64 .
