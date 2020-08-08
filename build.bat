@echo off

if "%1"=="" goto defaultrole
set role_name=%%1
goto endrole
:defaultrole
set role_name=PROGRAMMER
:endrole
set role=ROLE_%role_name% 
set application_executable=arcane_win32.exe 
set application_dll=arcane.dll 
set build_options= -DBUILD_WIN32=1 -DBUILD_DEBUG=1 -DBUILD_RELEASE=0 -DBUILD_DEVELOPER=1 -DROLE=%role%  -DTSFOUNDATION_PROGRAM_OPTIONS=\"program_options.h\"  -DTS2D=1  -DTSASSETS=1  -DTSDEVTERMINAL=1  -DTSINPUT=1  -DTSUI=1 
set common_compiler_flags= /TC /GL- -MTd -nologo -Gm- -GR- -Gs- -Gy- -Gw- -EHa- -Od -W3 -wd4201 -wd4100 -wd4189 /Zi -D_CRT_SECURE_NO_WARNINGS /FS /FC /I..\source\ %build_options% 
set platform_linker_flags= user32.lib gdi32.lib winmm.lib 
set common_linker_flags= -opt:ref -incremental:no /Debug:fastlink opengl32.lib     
if not exist game\build mkdir game\build
pushd game\build
del *.pdb > NUL 2> NUL
echo --- [Telescope Build] Building platform executable.
start /b /wait "" "cl" %common_compiler_flags% /Fdplatform.pdb ..\source\telescope\tsfoundation\tsfoundation_win32.c /link %common_linker_flags% %platform_linker_flags% /OUT:%application_executable%
popd
echo --- [Telescope Build] Building game layer.
call build_game_layer.bat %role_name%
echo --- [Telescope Build] Build completed.
