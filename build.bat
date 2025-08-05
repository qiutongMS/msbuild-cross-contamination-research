@echo off
echo Building MSBuild Preprocessor Definitions Cross-Contamination Test...
echo.

REM Build the project using MSBuild
msbuild TestProject.vcxproj /p:Configuration=Debug /p:Platform=Win32 /v:normal

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build succeeded! Running test executable...
    echo.
    .\TestProject\Debug\TestProject.exe
    echo.
    echo Running the executable...
    echo.
    .\x64\Debug\TestProject.exe
) else (
    echo.
    echo Build failed with error code %ERRORLEVEL%
)

pause
