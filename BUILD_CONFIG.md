# JNRenderer - 构建配置指南

## Premake5 配置完善说明

### 问题解决
**问题**：无法找到 `nlohmann/json.hpp` 头文件
```
error: 无法打开 源 文件 "nlohmann/json.hpp"
```

**原因**：
1. `Config.h` 使用 `#include <nlohmann/json.hpp>` 但包含路径未配置
2. Premake 生成的项目文件缺少必要的头文件搜索路径

### 解决方案

#### 1. 修改 Config.h
```cpp
// 旧：#include <nlohmann/json.hpp>
// 新：直接使用相对路径
#include "json.hpp"
```

#### 2. 完善 renderer/premake5.lua
添加到 `includedirs`：
- `thirdparty.."/spdlog"` - Spdlog 日志库
- 确保 `thirdparty.."/"`（3rdParty 根目录）包含在内

添加到 `files`：
- `thirdparty.."/json.hpp"` - 明确包含 JSON 库

#### 3. 完善 editor/premake5.lua  
添加到 `includedirs`：
- `thirdparty.."/glm"` - GLM 数学库
- `thirdparty.."/spdlog"` - Spdlog 日志库

### 生成的配置
```lua
-- renderer 项目
includedirs
{
    "./",
    thirdparty.."/glm",
    thirdparty.."/imgui",
    thirdparty.."/tiny_obj_loader",
    thirdparty.."/glad/include",
    thirdparty.."/glfw/include",
    thirdparty.."/stb",
    thirdparty.."/spdlog",
    thirdparty.."/",
}

files
{
    "**.cpp",
    "**.h",
    "**.hpp",
    "**.lua",
    thirdparty.."/glm/**.cpp",
    thirdparty.."/glm/**.hpp",
    -- ... 其他库文件 ...
    thirdparty.."/json.hpp",  -- 关键！
}
```

### 构建步骤

#### 方式 1：使用 build.bat 脚本（推荐）
```bash
G:\JNRenderer\JNRenderer> build.bat
```

#### 方式 2：手动编译
1. 生成项目文件：
```bash
.\premake5.exe vs2022
```

2. 编译 Debug 版本：
```bash
msbuild JNRenderer.sln /p:Configuration=Debug /p:Platform=x64
```

3. 编译 Release 版本：
```bash
msbuild JNRenderer.sln /p:Configuration=Release /p:Platform=x64
```

### 第三方库配置总结

| 库 | 位置 | 用途 | 状态 |
|---|------|------|------|
| GLM | `project/3rdParty/glm/` | 数学库 | ✓ 已配置 |
| GLFW | `project/3rdParty/glfw/` | 窗口/输入 | ✓ 已配置 |
| GLAD | `project/3rdParty/glad/` | OpenGL 加载器 | ✓ 已配置 |
| ImGui | `project/3rdParty/imgui/` | UI 框架 | ✓ 已配置 |
| spdlog | `project/3rdParty/spdlog/` | 日志库 | ✓ 已配置 |
| nlohmann/json | `project/3rdParty/json.hpp` | JSON 处理 | ✓ 已配置 |
| stb | `project/3rdParty/stb/` | 图像加载 | ✓ 已配置 |
| tiny_obj_loader | `project/3rdParty/tiny_obj_loader/` | OBJ 模型加载 | ✓ 已配置 |

### 编译验证检查清单
- [ ] 生成 Visual Studio 项目文件无错误
- [ ] 编译 Debug 版本成功
- [ ] 编译 Release 版本成功
- [ ] 输出文件位置：`OUTPUT/Debug-windows-x86_64/bin/`
- [ ] 编辑器可执行文件：`editor.exe`
- [ ] 渲染库 DLL：`renderer.dll`

### 常见问题

**Q: 找不到 PreprocessorDefinitions？**
A: 确保在 Premake 配置中定义了 `_JN_ENGINE`

**Q: 链接错误 - 找不到 renderer.lib？**
A: 确保先编译 renderer 项目，editor 依赖它

**Q: 运行时找不到 dll？**
A: Post-build 命令应将 renderer.dll 复制到 editor 目录

