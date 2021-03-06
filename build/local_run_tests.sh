#!/bin/bash
export CI_REGISTRY=local
export PROJECT=build
export VOLUME=..

if [ -z "$1" ]
then
    export VERSION=centos7_4_2_6
else
    export VERSION=$1
fi


docker-compose -f docker-compose-local.yml run icat /build/build/gitlab_test_in_container.sh

err=$?

echo "error code $err"

docker-compose -f docker-compose-local.yml down -v

exit $err
     
