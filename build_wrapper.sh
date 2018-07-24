#!/bin/bash

# determines the current branch and version and invokes build.sh in the container

branch=$( git rev-parse --abbrev-ref HEAD )

function get_tags {
    git tag --points-at HEAD | grep -e ^v | sed 's/^v//g'
}

if [ "$branch" == "master" ]
then
    n_versions=$( get_tags | wc -l )
    if [ "$n_versions" = 0 ]
    then
        echo "no tag found in master"
    elif [ "$n_versions" = 1 ]
    then
        version=$( get_tags )
        docker exec centos7_4_1_11_icat_1 /build/build.sh "" $version
    else
        echo "version tags not unique:"
        get_tags
        exit 8
    fi
else
    # version is timestamp for develop and feature branch
    version=$( git log -1 --date=raw | awk '{ if($1=="Date:") print $2; }' )
    docker exec centos7_4_1_11_icat_1 /build/build.sh $branch $version
fi


