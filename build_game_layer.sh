pushd game
mkdir -p game
pushd game
pwd
gcc -c ../source/arcane.c -I../source -I../source/telescope/ -fPIC -o arcane.so
popd
