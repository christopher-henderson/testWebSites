#/usr/bin/env bash

git clone https://github.com/nss-dev/nss.git
hg clone https://hg.mozilla.org/projects/nspr

cd nss
./build.sh -v
if [ $? -ne 0 ]
then
	echo "darnit"
	exit 1
else
	echo "well, it built just fine"
fi
cd ..

ls dist/Debug/lib/
# libfreebl3.chk        libmozpkix.a          libnss3.dylib.TOC     libnssdbm3.dylib      libplc4.a             libsectool.a          libsoftokn3.dylib     pkgconfig
# libfreebl3.dylib      libnspr4.a            libnssckbi.dylib      libnssdbm3.dylib.TOC  libplc4.dylib         libsmime3.dylib       libsoftokn3.dylib.TOC
# libfreebl3.dylib.TOC  libnspr4.dylib        libnssckbi.dylib.TOC  libnssutil3.dylib     libplds4.a            libsmime3.dylib.TOC   libssl3.dylib
# libmozpkix-testlib.a  libnss3.dylib         libnssdbm3.chk        libnssutil3.dylib.TOC libplds4.dylib        libsoftokn3.chk       libssl3.dylib.TOC
echo "^ libnss3.dylib is there alright"

# Come on, this is all I want.
echo "#include \"nss.h\"

int main(int argc, char const *argv[])
{
	NSS_Init(\"lok/tar/ogar\");
	return 0;
}
" > thing.cpp

# It'll even have dist/Debug/lib in the output for the linker's search path.
clang++ thing.cpp -L dist/Debug/lib/libnss3.dylib -I nss/lib/nss -I nss/lib/util -I dist/Debug/include/nspr/ -v

# And just to be thorough scrape through looking for the right symbol.
for lib in $(find dist/Debug/lib -name *.dylib); do
	objdump -D $lib | grep Now:; 
	if [ $? -eq 0 ]
	then
		echo "See? It's totally in $lib"
	fi
done