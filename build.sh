role_name=PROGRAMMER
role=ROLE_$role_name
application_executable=arcane
application_dll=arcane.so
# build_options= -DBUILD_WIN32=1 -DBUILD_DEBUG=1 -DBUILD_RELEASE=0 -DBUILD_DEVELOPER=1 -DROLE=%role%  -DTSFOUNDATION_PROGRAM_OPTIONS=\"program_options.h\"  -DTS2D=1  -DTSASSETS=1  -DTSDEVTERMINAL=1  -DTSINPUT=1 -DTSUI=1
# common_compiler_flags= /TC /GL- -MTd -nologo -Gm- -GR- -Gs- -Gy- -Gw- -EHa- -Od -W3 -wd4201 -wd4100 -wd4189 /Zi -D_CRT_SECURE_NO_WARNINGS /FS /FC /I..\source\ %build_options% 
platform_includes=-Igl\ -IglX 
# common_linker_flags= -opt:ref -incremental:no /Debug:fastlink opengl32.lib

mkdir -p game/build
pushd game/build
echo "Building Telescope..."
if clang -c ../source/telescope/tsfoundation/tsfoundation_linux.c -DBUILD_LINUX=1 -DBUILD_DEBUG=1 -DBUILD_DEVELOPER=1 -DROLE=PROGRAMMER -DTS2D=1 -DTASSETS=1 -DTSDEVTERMINAL=1 -DTSINPUT=1 -DTSUI=1 $platform_includes -o arcane; then
	echo "Build Success!"
	popd
	echo "Building Game Layer..."
	sh build_game_layer.sh
else
	echo "Build Failed!"
fi

