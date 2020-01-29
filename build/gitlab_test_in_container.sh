#!/bin/bash
set -x
set -e

cp -r /build /local-build
cd /local-build
make
make test
make install

cd /local-build/test
py.test



