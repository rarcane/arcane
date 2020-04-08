@echo off

if exist .\game\build (
    @RD /S /Q "game\build"
)

if exist .\data_desk_custom\build (
    @RD /S /Q "data_desk_custom\build"
)

if exist .\tools\data_desk\build (
    @RD /S /Q "tools\data_desk\build"
)

echo Successfully cleaned build folders.