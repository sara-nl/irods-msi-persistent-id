#!/bin/bash
export VOLUME=..
if [ -z "$1" ]
then
    export VERSION=centos7_4_2_6
else
    export VERSION=$1
fi

docker-compose -f docker-compose-local.yml down -v
