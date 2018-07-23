# Irods microservices

This repository contains the code and the build system for compiling iRods microservices.

## Submodules
The main repository contains several reusable git submodules:
* **Catch2**: a unit test framework for C++ (cloned from the public repository https://github.com/catchorg/Catch2)
* **handle-cpp-client**: a C++ client library for handle and reverse lookup API calls:
  * http://www.handle.net/tech_manual/HN_Tech_Manual_9.pdf
  * https://userinfo.surfsara.nl/systems/epic-pid/usage/handle-reverse-lookup
* **irods-ci-server**: Docker containers for building and testing iRods plugins. Ansible playbook to deploy the server.

The module _handle-cpp-client_ depends on other submodules:
* **CliArgs**: command line argument parser
* **json-parser-cpp**: json parser and object manipulation for C++ (based on [Boost Spirit](https://www.boost.org/doc/libs/1_64_0/libs/spirit/doc/html/index.html))

In order to pull in all dependend modules use the following git command:
```bash
git submodule update --init --recursive --remote
```

## Compilation of microservices
The microservices can be built and tested in docker containers.
All dependencies are prepared in the image. Hence, the only requirement for 
the build host is docker and docker-compose.
Multiple iRods versions are supported. Currently centos7 is supported as underlying OS.
It is straightforward to extend the build system to other systems like Ubuntu.

### building a single microservice
In order to build the word count microservice (a simple example for illustration purpose), follow 
the following steps. Other microservices are built in the same way.

1. Create a docker composite

``` bash
./irods-ci-server/up centos7_4_1_11 --vol .
```
The script set up the container composite for building and testing iRods (iRods version 4.1.11 on centos7) microservices
in the background (detached mode). At the end it attempts to connect to the iCat server.
The last log message for a successful setup is
```
iCAT container is ready
```
The volume of the main repository (irods-microservices) should be mounted as /build inside the container.
The exact path is determined from the *--vol* argument of the helper script *up*. It can be relative or 
absolute.

In general, it should be possible to build the microservices for iRods version 4.1.x and 4.2.x.

**Note**: In some case (re-) building the images might be required:

``` bash
cd irods-ci-server/centos7_4_1_11
# or cd irods-ci-server/centos7_4_2_3
docker-compose build
```

2. Compile a microservice's shared library
   The following step invokes make in the container and produces 
   the microservice shared library file:

        docker exec centos7_4_1_11_icat_1 make -C /build/msi_wordcount

     The produces file should be availble:

        ls msi_wordcount/libmsi_wordcount_4.1.11.so

3. Run the unit tests for the microservice

        docker exec centos7_4_1_11_icat_1 make -C /build/msi_wordcount test

4. Install the shared libraries inside the container

        docker exec centos7_4_1_11_icat_1 make -C /build/msi_wordcount install
        
5. Run functional test suite for the microservice
        
        docker exec centos7_4_1_11_icat_1 make -C /build/msi_wordcount functional_test

6. Build a RPM package for the microservice
   **TODO**
   
7. Shut down the composite

        ./irods-ci-server/down centos7_4_1_11

**Note:** For trouble shooting it is possible to invoke an interactive shell in the running container 
and execute build commands interactively in the the container:
``` bash
docker exec -ti centos7_4_1_11_icat_1 /bin/bash
```

**Note:**
These steps have been automated in .gitlab-ci.yml

