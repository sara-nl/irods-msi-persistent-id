#!/bin/bash
set -x
set -e
echo $REGISTRY
docker-compose -f build/docker-compose.yml pull
docker-compose -f build/docker-compose.yml run icat /build/build/gitlab_test_in_container.sh
