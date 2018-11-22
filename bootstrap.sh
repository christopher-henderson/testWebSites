#!/usr/bin/env bash

install_gyp() {
	git clone https://chromium.googlesource.com/external/gyp
	cd gyp
	./setup.py
}

which gyp
if [ $? -ne 0 ]; then
	install_gyp
fi

which ninja
if [ $? -ne 0 ]; then
	# Sorry, don't feel like cross platform right now
	brew install ninja
fi

git clone https://github.com/nss-dev/nss.git
hg clone https://hg.mozilla.org/projects/nspr

cd nss
./build.sh -v
if [ $? -ne 0 ]; then
	echo "build failed"
	exit 1
fi

cd ..
mkdir -p bin/lok/tar/ogar
cp dist/Debug/lib/libnspr4.dylib bin
cp dist/Debug/lib/libnss3.dylib bin
cp dist/Debug/lib/libnssutil3.dylib bin
cp dist/Debug/lib/libplc4.dylib bin
cp dist/Debug/lib/libplds4.dylib bin
