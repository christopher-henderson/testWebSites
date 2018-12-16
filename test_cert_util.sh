#/usr/bin/env bash

set -e

export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$(pwd)/dist/Debug/lib
CERTUTIL=dist/Debug/bin/certutil


# mkdir testdb

# $CERTUTIL -V wwwletsencryptorg.crt 

$CERTUTIL -A -n letsencrypt.org -t "P,p,p" -i wwwletsencryptorg.crt  -d testdb

$CERTUTIL -A -n expiredec.entrust.net -t "P,p,p" -i expiredecentrustnet.crt  -d testdb