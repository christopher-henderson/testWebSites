#/usr/bin/env bash

# export DYLD_LIBRARY_PATH=/Users/chris/Documents/Contracting/mozilla/testWebsites/dist/Debug/lib
# export DYLD_LIBRARY_PATH

BIN=$(pwd)/bin
OUT=cmd
rm -rf $BIN$OUT

# clang++ comp.cpp -o $BIN/$OUT -stdlib=libc++ -Wall -L dist/Debug/lib -lnss3 -I nss/lib/nss -I nss/lib/util -I nss/lib/mozpkix/include/pkix -I nss/lib/mozpkix/ -I /nss/mozpkix -I nss/lib/mozpkix/include/pkix -I nss/lib/mozpkix/include/pkix -I dist/Debug/include/nspr/
clang++ comp.cpp -o $BIN/$OUT \
	-Wall \
	-std=c++11 \
	-L dist/Debug/lib -lnss3 \
	-I dist/Debug/include/nspr \
	-I dist/public \
	-I dist/public/nss
	
	
