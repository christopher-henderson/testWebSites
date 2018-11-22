#/usr/bin/env bash

BIN=$(pwd)/bin
OUT=cmd
rm -rf $BIN$OUT

c++ comp.cpp -o $BIN/$OUT \
	-Wall \
	-std=c++11 \
	-L dist/Debug/lib -lnss3 -lmozpkix -lnspr4 \
	-I dist/Debug/include/nspr \
	-I dist/public \
	-I dist/public/nss
