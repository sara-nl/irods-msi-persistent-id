# Irods microservices

This repository contains the code for creating and managing persistent
identifiers (handle) with iRods microservices.

## Submodules
The main repository contains several reusable git submodules:
* **Catch2**: a unit test framework for C++
  (cloned from the public repository https://github.com/catchorg/Catch2)
* **handle-cpp-client**: a C++ client library for handle and reverse
  lookup API calls:
  * http://www.handle.net/tech_manual/HN_Tech_Manual_9.pdf
  * https://userinfo.surfsara.nl/systems/epic-pid/usage/handle-reverse-lookup
* **irods-ci-server**: Building system for iRods plugins. 

The module _handle-cpp-client_ depends on other submodules:
* **CliArgs**: command line argument parser
* **json-parser-cpp**: json parser and object manipulation for C++ 
  (based on [Boost Spirit](https://www.boost.org/doc/libs/1_64_0/libs/spirit/doc/html/index.html))

In order to pull in all dependend modules use the following git command:
```bash
git submodule update --init --recursive --remote
```

## Compilation of microservices
See instruction in the submodule _irdos-ci-server_.

## Installation
The continious integration process produces an RPM and updates a yum repository.
Add the yum repository to your enviornment:


    sudo cat > /etc/yum.repos.d/surfsara-irods.repo <<EOF
    [surfsara-irods]
    name=Local CentOS Repo
    baseurl=http://145.100.58.2/CentOS/7/irods-4.1.11/
    gpgcheck=0
    EOF
    
    # check the packages
    yum update
    yum list available --disablerepo "*" --enablerepo surfsara-irods
    
