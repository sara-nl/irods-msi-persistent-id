# Irods microservices

This repository contains the code for creating and managing persistent
identifiers (handle) with iRods microservices.

## Submodules
The main repository contains several reusable git submodules:
* **Catch2**: a unit test framework for C++ https://github.com/catchorg/Catch2
* **handle-cpp-client**: a C++ client library for handle and reverse
  lookup API calls (https://github.com/sara-nl/surfsara-handle-client-cpp)
  * http://www.handle.net/tech_manual/HN_Tech_Manual_9.pdf
  * https://userinfo.surfsara.nl/systems/epic-pid/usage/handle-reverse-lookup
* **irods-ci-server**: Building system for iRods plugins. 

The module _surfsara-handle-cpp-client_ depends on other submodules:
* **CliArgs** (https://github.com/sara-nl/surfsara-cli-args-cpp): command line argument parser
* **json-parser-cpp** (https://github.com/sara-nl/surfsara-json-parser-cpp): json parser and object manipulation for C++ 
  (based on [Boost Spirit](https://www.boost.org/doc/libs/1_64_0/libs/spirit/doc/html/index.html))

In order to pull in all dependend modules use the following git command:
```bash
git submodule update --init --recursive --remote
```

## Installation
The continious integration process produces an RPM and updates a yum repository.
Add the yum repository to your enviornment:


    sudo cat > /etc/yum.repos.d/surfsara-irods.repo <<EOF
    [surfsara-irods]
    name=Local CentOS Repo
    baseurl={{PATH_REPO}}/CentOS/7/irods-4.1.11/
    gpgcheck=0
    EOF
    
    # check the packages
    yum update
    yum list available --disablerepo "*" --enablerepo surfsara-irods
    
    # install the services
    yum install msi-persistent-id-master.x86_64

## Configuration

    cd /etc/irods/
    cp irods_pid.json.template irods_pid.json
    vi irods_pid.json

To test the microervices check the rule files in the test directory.

## Compilation testing and packaging
This repository contains a cross-platform build systems for different iRODS version for 
CentOS7. It is straightforward to add additional operating systems and iRODS versions.

Requirements to build, test and package the microservices:
* docker
* docker-compose (> 1.21).
  If the Linux system does not support this version, binary can be directly downloaded from 
  https://github.com/docker/compose/releases/download/1.21.2/docker-compose-Linux-x86_64


### Directions
1. Build the required docker images

``` bash
cd build/centos7_4_1_11
docker-compose build
cd -

cd build/centos7_4_2_3
docker-compose build
cd -

cd build/centos7_4_2_4
docker-compose build
cd -

```

2. Test the required containers

``` bash
./build/up ./build/centos7_4_1_11
./build/down ./build/centos7_4_1_11

./build/up ./build/centos7_4_2_3
./build/down ./build/centos7_4_2_3

./build/up ./build/centos7_4_2_4
./build/down ./build/centos7_4_2_4

```

3. Building the iRODS micorservices and perform unit tests

``` bash
./build/up ./build/centos7_4_1_11
./build/make ./build/centos7_4_1_11
./build/make ./build/centos7_4_1_11 test
```
and likewise for the other versions

4. Installing the binaries into the test container and perform functional tests

``` bash
./build/install ./build/centos7_4_1_11
./build/run_test ./build/centos7_4_1_11
```

5.  Create RPM

Create a RPM for the current branch
``` bash
./build/make_rpm --irods-version 4.1.11
```
Note the version is automatically determined:
* if the current git HEAD is a tag in the form _vX.Y.Z_, the version of the PDF is 
  generated as X.Y.Z
* otherwise the verions is determined as _X.Y_, where _X_ is the current branch (e.g. _master_) and 
  _Y_ is the unix timestamp of the last commit.

The version number of the RPM can also be overwritten
``` bash
./build/make_rpm --irods-version 4.1.11 --version 1.2.0
```

The RPMs are written to the directory RPMS/

6. Make clean

``` bash
./build/make ./build/centos7_4_1_11 clean
```

