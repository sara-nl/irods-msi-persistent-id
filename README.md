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

## Usage

The package provides different microservices for creating, updating, deleting, searching handles.
Some of them come in two flavors, accepting an iRODS path or a handle as input.
In the first case the irods path is converted into a handle by a reverse lookup before 
the actual operation is performed.

### msiPidCreate

* **operation**: Create a new PID based on the configured handle profile.
* **input**:     iRODS path, list of keys-values pairs
* **output**:    handle
* **example**:   [rule_create.r](test/rule_create.r)


### msiPidGet

* **operation**: Resolves iRODS to PID and retrieves information from the handle server.
  If key is empty, the full JSON string is returned. Otherwise the value
  of the specific key.
* **input**:
  * iRODS path
  * key
* **output**: json or value of a ceratin key
* **example**:   [rule_get.r](test/rule_get.r)

### msiPidGetHandle

* **operation**: Retrieves informat from the handle server.
  If key is empty, the full JSON string is returned. Otherwise the value
  of the specific key.
* **input**:
  * handle
  * key
* **output**: json string or value of the key
* **example**:   [rule_get_handle.r](test/rule_get_handle.r)

### msiPidSet
* **operation**: Resolve iRODS path and set the value of a key in the handle.
  Multiple values can be set by passing an list of alternating key values as key.
* **input**: 
  * iRODS path
  * key
  * value
* **output**: the resolved handle
* **example**:   [rule_set.r](test/rule_set.r)

### msiPidSetHandle
* **operation**: Set the value of a key in the handle.
  Multiple values can be set by passing an list of alternating key values as key.
* **input**: 
  * handle
  * key
  * value
* **output**: the resolved handle
* **example**:   [rule_set_handle.r](test/rule_set_handle.r)

### msiPidUnset
* **operation**: 
* **input**: 
* **output**:
* **example**:   [rule_unset.r](test/rule_unset.r)


### msiPidUnsetHandle
* **operation**: 
* **input**: 
* **output**:
* **example**:   [rule_unset_handle.r](test/rule_unset_handle.r)

### msiPidMove
* **operation**: 
* **input**: 
* **output**:
* **example**:   [rule_move.r](test/rule_move.r)

### msiPidMoveHandle
* **operation**: 
* **input**: 
* **output**:
* **example**:   [rule_move_handle.r](test/rule_move_handle.r)

### msiPidDelete
* **operation**: 
* **input**: 
* **output**:
* **example**:   [rule_delete.r](test/rule_delete.r)

### msiPidDeleteHandle
* **operation**: 
* **input**: 
* **output**:
* **example**:   [rule_delete_handle.r](test/rule_delete_handle.r)

### msiPidLookup
* **operation**: 
* **input**: 
* **output**:
* **example**:   [rule_lookup.r](test/rule_lookup.r)

### msiPidLookupOne
* **operation**: 
* **input**: 
* **output**:
* **example**:   [rule_lookup_one.r](test/rule_lookup_one.r)

### msiPidLookupKey
* **operation**: 
* **input**: 
* **output**:
* **example**:   [rule_lookup_key.r](test/rule_lookup_key.r)


## Compilation testing and packaging
This repository contains a cross-platform build systems for different iRODS version for 
CentOS7. It is straightforward to add additional operating systems and iRODS versions.

Requirements to build, test and package the microservices:
* docker
* docker-compose (> 1.21).
  If the Linux system does not support this version, the binary can be directly downloaded from 
  https://github.com/docker/compose/releases/download/1.21.2/docker-compose-Linux-x86_64


### Directions
#### 1. Build the required docker images

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

#### 2. Test the required containers

``` bash
./build/up ./build/centos7_4_1_11
./build/down ./build/centos7_4_1_11

./build/up ./build/centos7_4_2_3
./build/down ./build/centos7_4_2_3

./build/up ./build/centos7_4_2_4
./build/down ./build/centos7_4_2_4

```

#### 3. Building the iRODS micorservices and perform unit tests

``` bash
./build/up ./build/centos7_4_1_11
./build/make ./build/centos7_4_1_11
./build/make ./build/centos7_4_1_11 test
```
and likewise for the other versions

#### 4. Installing the binaries into the test container and perform functional tests

``` bash
./build/install ./build/centos7_4_1_11
./build/run_test ./build/centos7_4_1_11
```

#### 5.  Create RPM

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

