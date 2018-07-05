# Irods microservices

## Initialize submodules

```bash
git submodule update --init --recursive --remote
```

## Compilation of microservices

1. Create a docker composite

``` bash
./irods-ci-server/up centos7_4_1_11 --vol .
```

2. Compile a microservice and running several test manually

``` bash
docker exec centos7_4_1_11_icat_1 make -C /build/msi_wordcount
docker exec centos7_4_1_11_icat_1 make -C /build/msi_wordcount test
docker exec centos7_4_1_11_icat_1 make -C /build/msi_wordcount install
docker exec centos7_4_1_11_icat_1 make -C /build/msi_wordcount functional_test
docker exec centos7_4_1_11_icat_1 make -C /build/msi_wordcount package
```

3. Invoke interactive shell for trouble shooting

``` bash
docker exec -ti centos7_4_1_11_icat_1 /bin/bash
```

4. Shut down composite

``` bash
./irods-ci-server/down centos7_4_1_11
```

*Note*:
These steps have been automated in .gitlab-ci.yml

