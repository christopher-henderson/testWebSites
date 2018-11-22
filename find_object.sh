#/usr/bin/env bash

# And just to be thorough scrape through looking for the right symbol.
for lib in $(find dist/Debug/lib -name *.dylib); do
	objdump -D $lib | grep $1:; 
	if [ $? -eq 0 ]
	then
		echo "See? It's totally in $lib"
	fi
done