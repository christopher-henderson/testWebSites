#/usr/bin/env bash

# This helped me figure our what libraries things were getting dumped to without
# having to dig into the build system too much.

for lib in $(find dist/Debug/lib -name *.dylib); do
	objdump --arch-name=x86-64 -D $lib | grep $1:; 
	if [ $? -eq 0 ]
	then
		echo "See? It's totally in $lib"
	fi
done