@echo off



rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem                                  Load Role
rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if "%1"=="" goto defaultrole
set role_name=%1
goto endrole
:defaultrole
:endrole



rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem                                Build Options
rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
set role=ROLE_%role_name%
set application_executable=arcane.exe
set application_dll=arcane.dll
set build_options= -DBUILD_WIN32=1 -DTSFOUNDATION_PROGRAM_OPTIONS=\"tsarcane/arcane_program_options.h\" -DTSFOUNDATION_WIN32_FILE=\"tsarcane/arcane_tsfoundation_win32.c\" -DTSFOUNDATION_MODULES_DECLARATION_FILE=\"tsarcane/arcane_tsfoundation_modules_declaration.h\" -DTSUI_STYLE_CALLS_FILE=\"tsarcane/arcane_tsui_style_calls.inc\" -DROLE=%role% -I..\..\game\sauce
set common_compiler_flags= /TC /GL- -MTd -nologo -Gm- -GR- -Gs- -Gy- -Gw- -EHa- -Od -W3 -wd4201 -wd4100 -wd4189 /Zi -D_CRT_SECURE_NO_WARNINGS /FS %build_options%
set common_linker_flags= -opt:ref -incremental:no /Debug:fastlink
set platform_linker_flags= user32.lib gdi32.lib winmm.lib opengl32.lib



rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem                               Make Directories
rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if not exist .\game\build mkdir game\build
if not exist .\data_desk_custom\build mkdir data_desk_custom\build
if not exist .\data_desk_custom\generated mkdir data_desk_custom\generated



rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem                                Build Data Desk
rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pushd tools\data_desk
del *pdb > NUL 2> NUL
echo --- Compiling Data Desk.
call build.bat
popd



rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem                         Build Data Desk Custom Layer
rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pushd data_desk_custom\build
del *pdb > NUL 2> NUL
echo --- Compiling Data Desk custom layer.
start /b /wait "" "cl" %common_compiler_flags% ..\source\arcane_data_desk.c -I ..\..\tools\data_desk\source\ /LD /link %common_linker_flags% %platform_linker_flags% /OUT:arcane_data_desk.dll
popd



rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem                             Run Data Desk pass
rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pushd data_desk_custom\generated
set files= 
for %%i in (..\data\*.ds) do ( call set "files=%%files%% %%i" )
echo --- Running Data Desk pass and generating code.
start /b /wait "" "..\..\tools\data_desk\build\data_desk.exe" --custom ..\build\arcane_data_desk.dll %files%
start /b /wait "" "xcopy" ..\generated ..\..\game\sauce\generated\ /y /s /e /q
popd



rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem                           Build Platform Exectuable
rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pushd game\build
del *.pdb > NUL 2> NUL
echo --- Compiling platform executable.
start /b /wait "" "cl" %common_compiler_flags% /Fdplatform.pdb ..\sauce\tsfoundation\tsfoundation_win32.c -I sauce\tsfoundation /link %common_linker_flags% %platform_linker_flags% /OUT:%application_executable%
popd



rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem                          Build Application Layer
rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pushd game\build
del *.pdb > NUL 2> NUL
echo --- Compiling application layer.
start /b /wait "" "cl" %common_compiler_flags% ..\sauce\arcane.c -I ..\sauce\tsfoundation /LD /link %common_linker_flags% %platform_linker_flags% /OUT:%application_dll%
popd



rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rem                             Copy Resources
rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo --- Copying resources to build.
if exist game\build\res @RD /S /Q game\build\res
mkdir game\build\res
robocopy game\res game\build\res /E > NUL



echo --- Build completed.