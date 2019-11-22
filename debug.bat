@echo off

call build.bat
echo --- Launching Visual Studio Development Environment
devenv game/build/arcane.exe