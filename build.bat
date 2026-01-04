@echo off
REM 编译验证脚本
REM 设置 Visual Studio 2022 环境变量

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

REM 编译 Debug 版本
echo.
echo ========================================
echo Starting Debug Build...
echo ========================================
cd /d G:\JNRenderer\JNRenderer
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" JNRenderer.sln /p:Configuration=Debug /p:Platform=x64 /verbosity:minimal

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Debug build FAILED!
    exit /b 1
)

echo.
echo ========================================
echo Debug Build SUCCESS!
echo ========================================

REM 编译 Release 版本
echo.
echo ========================================
echo Starting Release Build...
echo ========================================
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" JNRenderer.sln /p:Configuration=Release /p:Platform=x64 /verbosity:minimal

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Release build FAILED!
    exit /b 1
)

echo.
echo ========================================
echo Release Build SUCCESS!
echo ========================================
echo.
echo All builds completed successfully!
