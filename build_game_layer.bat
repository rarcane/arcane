@echo off

rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
for /f "delims=:. tokens=1-4" %%t in ("%TIME: =0%") do (
set pdb_filename=arcane_%%t%%u%%v%%w.pdb
)

rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pushd tools
pushd data_desk
call build.bat
popd
popd

rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pushd data_desk_custom
pushd build
echo --- Building Data Desk custom layer.
start /b /wait "" "cl.exe" -nologo /Zi /FC -I../../tools/data_desk/source/ ../source/arcane_data_desk.c /LD
popd
popd

rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pushd data_desk_custom
if not exist generated mkdir generated
pushd generated
set files= 
for %%i in (..\data\*.ds) do ( call set "files=%%files%% %%i" )
echo --- Running Data Desk pass and generating code.
start /b /wait "" "..\..\tools\data_desk\build\data_desk.exe" --custom ..\build\arcane_data_desk.dll %files%
start /b /wait "" "xcopy" ..\generated ..\..\game\source\generated\ /y /s /e /q
popd
popd

rem  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pushd game
start /b "" "xcopy" .\res .\build\res\ /y /s /e /q
pushd build
del *.pdb > NUL 2> NUL
start /b /wait "" "cl" %common_compiler_flags% ..\source\arcane.c -I..\source -I..\source\telescope\ /LD /link /PDB:%pdb_filename% %common_linker_flags% -incremental:no /OUT:%application_dll%
popd
popd
