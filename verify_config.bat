REM 快速验证脚本 - 检查文件和路径
REM ==========================================

echo.
echo 检查关键文件是否存在...
echo.

if exist "project\3rdParty\json.hpp" (
    echo [OK] json.hpp found at project\3rdParty\
) else (
    echo [ERROR] json.hpp NOT found!
)

if exist "project\3rdParty\spdlog\spdlog.h" (
    echo [OK] spdlog.h found
) else (
    echo [ERROR] spdlog.h NOT found!
)

if exist "project\3rdParty\glm\glm.hpp" (
    echo [OK] glm.hpp found
) else (
    echo [ERROR] glm.hpp NOT found!
)

if exist "project\src\renderer\Base\Config.h" (
    echo [OK] Config.h found
) else (
    echo [ERROR] Config.h NOT found!
)

if exist "project\src\renderer\premake5.lua" (
    echo [OK] renderer/premake5.lua found
) else (
    echo [ERROR] renderer/premake5.lua NOT found!
)

echo.
echo 检查 Premake 配置...
echo.

call ".\premake5.exe" vs2022 --file="project\src\renderer\premake5.lua" --test 2>&1 | find /i "error" && (
    echo [ERROR] Premake configuration error detected!
) || (
    echo [OK] Premake configuration validated
)

echo.
echo 验证完成!
echo.
