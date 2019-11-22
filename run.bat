@echo off

if exist .\game\build\Arcane.exe (
    @echo on
    call game\build\Arcane.exe
) else (
    echo "Arcane excecutable not found. Please build."
)