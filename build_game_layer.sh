pushd tools
pushd data_desk
sh build.sh
popd
popd

pushd data_desk_custom
mkdir -p build
pushd build
clang -c ../source/arcane_data_desk.c -I../../tools/data_desk/source/ -fPIC
popd
popd

pushd data_desk_custom
mkdir -p generated
pushd generated
cp ../data/*.ds ./
files=$(ls ../data/)
../../tools/data_desk/build/data_desk --custom ../build/arcane_data_desk.o $files
cp -r ../generated/* ../../game/source/generated
popd
popd

pushd game
mkdir -p build
pushd build
mkdir -p res
cp -r ../res/* ./res
clang -w -c ../source/arcane.c -m64 -IGL -I../source -I../source/telescope/ -fPIC -o arcane.so -DTSUI=1 -DTS2D=1 -DTSASSETS=1 -DTSDEVTERMINAL=1 -DTSINPUT=1 -DBUILD_LINUX=1 -DBUILD_DEVELOPER=1 -DBUILD_RELEASE=0 -DBUILD_DEBUG=1 -DROLE=PROGRAMMER -DTSFOUNDATION_PROGRAM_OPTIONS=\"program_options.h\"
popd
