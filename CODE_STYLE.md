# JNRenderer 代码风格指南

## 概述
本指南定义JNRenderer项目的代码风格，确保代码一致性、可读性和可维护性。基于C++17标准，遵循现代C++最佳实践。

## 命名约定

### 文件命名
- 头文件：`.h`，类名一致（如`Camera.h`）。
- 源文件：`.cpp`，对应头文件。
- 目录：PascalCase（如`RenderPasses`）。

### 类和结构体
- 类名：PascalCase（如`RenderSystem`）。
- 结构体：PascalCase，与类相同。
- 成员变量：m_前缀 + camelCase（如`m_pShaderModule`）。
- 静态成员：s_前缀。

### 函数和方法
- 函数名：PascalCase（如`Init()`）。
- 私有方法：下划线前缀（如`_UpdateRenderContext()`）。
- 参数：camelCase（如`renderWidth`）。

### 变量
- 局部变量：camelCase（如`lightDir`）。
- 常量：UPPER_CASE（如`MAX_LIGHTS`）。
- 指针/引用：p前缀（如`pShader`）。

### 宏
- 宏：UPPER_CASE，下划线分隔（如`JNAPI`）。

## 代码格式

### 缩进
- 使用4个空格缩进，不使用Tab。
- 连续行对齐。

### 大括号
- K&R风格：左大括号与控制语句同行。
```cpp
if (condition) {
    // code
}
```

### 空格
- 运算符前后空格：`a + b`。
- 逗号后空格：`func(a, b)`。
- 括号内无空格：`if (a > b)`。

### 空行
- 函数间空一行。
- 类成员组间空一行。
- 逻辑块间空一行。

### 注释
- 单行：`// 注释`。
- 多行：`/* 注释 */`。
- 函数前Doxygen风格：
```cpp
/**
 * @brief 函数描述
 * @param param 参数描述
 * @return 返回值描述
 */
```

### 头文件保护
- `#pragma once`。

### 包含顺序
1. 对应头文件。
2. 标准库。
3. 第三方库。
4. 项目内部头文件。

```cpp
#include "MyClass.h"
#include <vector>
#include <glm/glm.hpp>
#include "Base/core.h"
```

## 最佳实践

### RAII
- 使用智能指针：`std::unique_ptr`、`std::shared_ptr`。
- 避免裸指针，除非必要。

### 异常
- 避免异常，使用返回值或断言。
- 关键错误用`assert`或日志。

### 常量
- 使用`constexpr`定义编译时常量。
- 避免魔法数字，用命名常量。

### 模板和泛型
- 使用模板时，提供清晰注释。
- 避免过度模板化。

### OpenGL
- 封装GL调用，添加错误检查。
- 使用VAO/VBO高效渲染。

### 日志
- 使用spdlog：`JNLOGINFO`、`JNLOGERROR`等。
- 关键操作添加日志。

### 性能
- 避免不必要拷贝，使用引用。
- 循环中避免函数调用。
- 考虑缓存友好性。

## 工具和检查
- 使用Clang-Format格式化代码。
- 启用编译器警告（-Wall -Wextra）。
- 定期代码审查。

## 示例

```cpp
#pragma once
#include "Base/core.h"
#include <memory>

class JNAPI MyClass
{
private:
    std::shared_ptr<ShaderModule> m_pShaderModule;
    int m_nWidth;

public:
    MyClass();
    ~MyClass();

    void Init(int width, int height);
    void Render();

private:
    void _SetupBuffers();
};
```

遵循此指南，确保代码质量和团队协作效率。