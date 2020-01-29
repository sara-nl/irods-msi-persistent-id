#!/bin/bash
set -x
set -e

echo $CI_REGISTRY_PASSWORD | docker login -u $CI_REGISTRY_USER --password-stdin  $CI_REGISTRY

docker-compose -f build/docker-compose.yml pull
# docker-compose -f build/docker-compose.yml run icat /build/build/gitlab_test_in_container.sh
docker-compose -f build/docker-compose.yml up

docker-compose -f build/docker-compose.yml down
