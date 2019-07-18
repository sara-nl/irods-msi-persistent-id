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

There are two configuration templates that can be copied and adjusted:
1. `irods_pid.json.01_default_profile` contains all fields that are required to connect to the servers.
 The handle profile is implicitly defined and includes the following fields:
 * `URL`
 * `IRODS/WEBDAV_URL`
 * `IRODS/URL`
 * `IRODS/SERVER`
 * `IRODS/SERVER_PORT`
 * `HS_ADMIN`

(the default profile is predefined in the [handle submodule](https://github.com/sara-nl/surfsara-handle-client-cpp/blob/master/doc/default_profile.json) )

2. `irods_pid.json.02_custom_profile`
  This example configuration includes a template for a custom handle profile.
  The fields of the profile can be configured as desired.

``` bash
    cd /etc/irods/
    cp irods_pid.json.01_default_profile irods_pid.json
    # or irods_pid.json.02_custom_profile
    vi irods_pid.json
```

### Configure permission
Each microservices requires certain permissions (*read*, *write*, *create* and *delete*)
that are configured in the permissions section
of the configuration file. Permissions can be granted to users, groups or everybody.
Note that using groups to check the permissions might slow down the microservice due to additional queries.
Permissions are checked in the following order:

* check if permission is granted to everybody
* check if permission is granted to user
* check if permission is granted to groups

Examples:
1. Everyone can read handles and perform reverse lookup.
```
"users_read": ["*"]
```

2. Only user "admin" from zone "myZone" can create handles

```
"users_create": ["admin#myZone"],
"groups_create": []
```

3. Every user in group "handle" can modify handles

```
"users_write": [],
"groups_write": ["handle"]
```

The permissions settings can be configured in the section "permissions" in the
main configuration file 
```
/etc/irods/irods_pid.json
```
For example to allow everybody read and lookup handles:

``` json
{
  "handle": {...},
  "lookup": {...},
  "permissions":{
    "users_read": ["*"],
    "groups_read": ["*"],
    "users_write": [],
    "groups_write": [],
    "users_create": [],
    "groups_create": [],
    "users_delete": [],
    "groups_delete": []
  },
```


### Test the configuration
To test the microervices apply the rule files in the test directory.

## Usage

The package provides different microservices for creating, updating, deleting, searching handles.
Some of them come in two flavors, accepting an iRODS path or a handle as input.
In the first case the irods path is converted into a handle by a reverse lookup before 
the actual operation is performed.

### msiPidCreate

* **operation**: Create a new PID based on the configured handle profile.
* **input**:     iRODS path, list of keys-values pairs
* **output**:    handle
* **required permission**: create
* **example**:   [rule_create.r](test/rule_create.r)


### msiPidGet

* **operation**: Resolves iRODS to PID and retrieves information from the handle server.
  If key is empty, the full JSON string is returned. Otherwise the value
  of the specific key.
* **input**:
  * iRODS path
  * key
* **output**: json or value of a ceratin key
* **required permission**: read
* **example**:   [rule_get.r](test/rule_get.r)

### msiPidGetHandle

* **operation**: Retrieves informat from the handle server.
  If key is empty, the full JSON string is returned. Otherwise the value
  of the specific key.
* **input**:
  * handle
  * key
* **output**: json string or value of the key
* **required permission**: read
* **example**:   [rule_get_handle.r](test/rule_get_handle.r)

### msiPidSet
* **operation**: Resolve iRODS path and set the value of a key in the handle.
  Multiple values can be set by passing an list of alternating key values as key.
* **input**: 
  * iRODS path
  * key
  * value
* **output**: the resolved handle
* **required permission**: write
* **example**:   [rule_set.r](test/rule_set.r)

### msiPidSetHandle
* **operation**: Set the value of a key in the handle.
  Multiple values can be set by passing an list of alternating key values as key.
* **input**: 
  * handle
  * key
  * value
* **output**: the resolved handle
* **required permission**: write
* **example**:   [rule_set_handle.r](test/rule_set_handle.r)

### msiPidUnset
* **operation**: Resolve iRODS path and remove a key from the handle
* **input**:
  * iRODS path
  * key
* **output**: the resolved handle involved
* **required permission**: write
* **example**:   [rule_unset.r](test/rule_unset.r)


### msiPidUnsetHandle
* **operation**: Remove a key from a handle
* **input**: 
  * handle
  * key
* **required permission**: write
* **example**:   [rule_unset_handle.r](test/rule_unset_handle.r)

### msiPidMove
* **operation**: Resolves handle and update all fields that incorporte the iRODS path
  (by the definition of the handle profile).
* **input**: 
  * iRODS path
  * new iRODS path
* **output**: resolved handle
* **required permission**: write
* **example**:   [rule_move.r](test/rule_move.r)

### msiPidMoveHandle
* **operation**: Update all fields tha incorporate the iRODS path
  (by the definition of the handle profile).
* **input**:
  * handle
  * new iRODS path
* **required permission**: write
* **example**:   [rule_move_handle.r](test/rule_move_handle.r)

### msiPidDelete
* **operation**: Resolve a handle and remove the entry
* **input**: 
  * iRODS path
* **output**: the handle removed
* **required permission**: delete
* **example**:   [rule_delete.r](test/rule_delete.r)

### msiPidDeleteHandle
* **operation**: Remove a handle
* **input**: handle
* **required permission**: delete
* **example**:   [rule_delete_handle.r](test/rule_delete_handle.r)

### msiPidLookup
* **operation**: Resolves a handle by a iRODS path. The path can contain wildcard characters.
* **input**: iRODS path (or pattern)
* **output**: list of handles
* **required permission**: read
* **example**:   [rule_lookup.r](test/rule_lookup.r)

### msiPidLookupOne
* **operation**: Attempts to resolve exactly one iRODS path. Fails if none or more than one 
 handles have been resolved.
* **input**: iRODS path or pattern
* **output**: handle
* **required permission**: read
* **example**:   [rule_lookup_one.r](test/rule_lookup_one.r)

### msiPidLookupKey
* **operation**: Resolve handle by arbitrary keys-value pair matching.
* **input**: 
  * key
  * value
* **output**: list of handles
* **required permission**: read
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

cd build/centos7_4_1_12
docker-compose build
cd -

cd build/centos7_4_2_3
docker-compose build
cd -

cd build/centos7_4_2_4
docker-compose build
cd -

cd build/centos7_4_2_5
docker-compose build
cd -

cd build/centos7_4_2_6
docker-compose build
cd -

```

#### 2. Test the required containers

``` bash
./build/up ./build/centos7_4_1_11
./build/down ./build/centos7_4_1_11

./build/up ./build/centos7_4_1_12
./build/down ./build/centos7_4_1_12

./build/up ./build/centos7_4_2_3
./build/down ./build/centos7_4_2_3

./build/up ./build/centos7_4_2_4
./build/down ./build/centos7_4_2_4

./build/up ./build/centos7_4_2_5
./build/down ./build/centos7_4_2_5

./build/up ./build/centos7_4_2_6
./build/down ./build/centos7_4_2_6
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
or configure it manually
``` bash
./build/make_rpm --irods-version 4.1.11 --branch master
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


#### 6. Make clean

``` bash
./build/make ./build/centos7_4_1_11 clean
```

