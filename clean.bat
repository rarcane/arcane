@echo off

if exist .\game\build (
    @RD /S /Q "game\build"
    echo Successfully cleaned build folder.
)