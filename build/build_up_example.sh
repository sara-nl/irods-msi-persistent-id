(
    export PROJECT=data-management-services
    export VERSION=centos7_4_2_6
    export VOLUME=..
    export CI_REGISTRY=git.ia.surfsara.nl:5050

    docker-compose -f docker-compose.yml pull
    docker-compose -f docker-compose.yml up
)
    
