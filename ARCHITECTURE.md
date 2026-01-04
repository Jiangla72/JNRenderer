# JNRenderer 架构优化分析与改造说明

## 架构优化点分析

### 1. 模块化渲染管道
- **优化前**：渲染流程硬编码在RenderSystem中，难以扩展。
- **优化后**：引入IPass抽象基类，RenderSystem管理多个Pass（DeferredPass、LightingPass、ShadowPass）。支持动态添加/移除Pass，提高灵活性。
- **优势**：易于添加新渲染技术，如后处理、PBR等。

### 2. 配置管理外部化
- **优化前**：路径和设置硬编码在代码中，修改需重新编译。
- **优化后**：使用JSON配置文件（config.json），Config类加载和管理配置。支持相对路径，提高可移植性。
- **优势**：运行时调整设置，无需编译；环境无关。

### 3. 渲染功能增强
- **多光源支持**：扩展LightingPass支持循环计算多个光源，添加光衰减公式，提升真实感。
- **阴影映射**：新增ShadowPass，实现方向光阴影，使用PCF过滤。集成到LightingPass中。
- **优势**：基础渲染器功能更完整，支持复杂场景。

### 4. 代码质量提升
- **错误处理**：添加资源加载验证和OpenGL错误检查。
- **注释修复**：修复中文注释乱码，确保可读性。
- **命名空间**：使用JNAPI等宏，避免全局污染。
- **优势**：代码更健壮、可维护。

### 5. 未实现的优化建议
- **ECS架构**：建议用EnTT替换简单Scene，提高实体管理效率。
- **异步资源加载**：使用线程池加载模型/纹理。
- **渲染抽象层**：引入IRenderer接口，支持多后端。
- **性能优化**：视锥剔除、LOD、GPU实例化。

## 改造后的目录结构

```
JNRenderer/
├── config.json                    # 配置文件（新增）
├── premake5.lua                   # 构建脚本
├── JNRenderer.sln                 # Visual Studio解决方案
├── models/                        # 模型资源
├── shaders/                       # 着色器文件
├── OUTPUT/                        # 构建输出
├── project/
│   ├── 3rdParty/                  # 第三方库
│   │   ├── glad/                  # OpenGL加载器
│   │   ├── glfw/                  # 窗口管理
│   │   ├── glm/                   # 数学库
│   │   ├── imgui/                 # UI库
│   │   ├── json.hpp               # JSON库（新增）
│   │   ├── spdlog/                # 日志库
│   │   ├── stb/                   # 图像加载
│   │   └── tiny_obj_loader/       # OBJ加载
│   └── src/
│       ├── editor/                # 编辑器模块
│       │   ├── Editor.cpp/h       # 编辑器主类
│       │   ├── Widget/            # UI组件
│       │   └── logs/              # 日志文件
│       └── renderer/              # 渲染器模块
│           ├── Base/              # 基础组件
│           │   ├── Config.cpp/h   # 配置管理（新增）
│           │   ├── core.h         # 核心宏
│           │   ├── Input.cpp/h    # 输入处理
│           │   ├── ISystem.cpp/h  # 系统接口
│           │   ├── LogSystem.cpp/h# 日志系统
│           │   ├── Timer.cpp/h    # 时间管理
│           │   └── JNInterface.h  # 接口定义
│           ├── engine.cpp/h       # 引擎核心
│           ├── EngineEntryPoint.h # 入口点
│           ├── EngineInterface.h  # 引擎接口
│           ├── Render/            # 渲染组件
│           │   ├── Camera.cpp/h   # 相机
│           │   ├── Light.cpp/h    # 光源
│           │   ├── Mesh.cpp/h     # 网格
│           │   ├── Model.cpp/h    # 模型
│           │   ├── Object.h       # 对象基类
│           │   ├── RenderPasses/  # 渲染Pass
│           │   │   ├── IPass.h    # Pass接口
│           │   │   ├── DeferredPass.cpp/h # 延迟渲染Pass
│           │   │   ├── LightingPass.cpp/h # 光照Pass
│           │   │   ├── ShadowPass.cpp/h   # 阴影Pass（新增）
│           │   │   └── DefaltForwardPass.cpp/h # 前向渲染Pass
│           │   ├── RenderSystem.cpp/h # 渲染系统
│           │   ├── Shader.cpp/h   # 着色器
│           │   ├── ShaderModule.cpp/h # 着色器模块
│           │   ├── SoftRenderer.cpp/h # 软件渲染器
│           │   ├── Texture.cpp/h  # 纹理
│           │   ├── Triangle.cpp/h # 三角形
│           │   └── Vertex.cpp/h   # 顶点
│           ├── Resource/          # 资源管理
│           ├── Scene/             # 场景管理
│           │   ├── Scene.cpp/h    # 场景
│           │   ├── SceneManager.cpp/h # 场景管理器
│           │   └── UI/            # UI组件
│           ├── UI/                # UI系统
│           ├── Util/              # 工具类
│           └── Window/            # 窗口系统
│               └── window.cpp/h   # 窗口管理
└── ARCHITECTURE.md                # 架构详细说明（本文档）
└── CODE_STYLE.md                  # 代码风格指南
```

## 架构详细说明

### 整体架构
JNRenderer采用模块化设计，核心是单例Engine类，管理多个ISystem子系统。渲染基于延迟着色管道，使用GLFW+GLAD+OpenGL。

### 核心模块
- **Engine**：主循环管理，初始化系统，处理更新/渲染/GUI/事件。
- **RenderSystem**：渲染管线管理，协调多个RenderPass。
- **SceneManager**：场景生命周期，管理当前场景。
- **ResourceManager**：资源缓存，加载模型/纹理/着色器。
- **Window**：GLFW窗口，处理输入和调整大小。
- **Editor**：ImGui界面，场景编辑。

### 渲染管道
1. **DeferredPass**：几何Pass，渲染到G-Buffer（位置、法线、颜色+镜面）。
2. **ShadowPass**：阴影Pass，生成深度贴图。
3. **LightingPass**：光照Pass，采样G-Buffer和阴影，计算最终颜色。

### 配置系统
Config类加载JSON配置，提供静态访问方法。支持运行时调整路径和参数。

### 第三方依赖
- GLM：向量/矩阵数学。
- ImGui：UI渲染。
- tiny_obj_loader：OBJ模型解析。
- stb_image：纹理加载。
- spdlog：日志记录。
- nlohmann/json：JSON解析（新增）。

### 扩展性
- 新Pass继承IPass，注册到RenderSystem。
- 新系统继承ISystem，注册到Engine。
- 配置驱动，无硬编码。

此架构支持从小项目扩展到复杂渲染器，强调模块化和可配置性。

## TODO: 缺失功能实现方案

### 已有的功能总结
- **基础渲染**：OpenGL窗口、着色器、纹理、模型加载（OBJ）。
- **延迟渲染管道**：G-Buffer几何Pass、光照Pass、阴影Pass。
- **光照系统**：多光源支持、衰减、方向光阴影映射。
- **配置管理**：JSON外部配置，路径和参数可配置。
- **性能优化**：视锥剔除、渲染抽象层（IRenderer）。
- **代码质量**：风格统一、注释完善、错误处理。

### 缺失的功能总结
- **PBR渲染**：物理-based材质、环境光遮蔽。
- **后处理效果**：Bloom、Tone Mapping、FXAA抗锯齿。
- **动画系统**：骨骼动画、关键帧插值。
- **物理集成**：碰撞检测、刚体模拟。
- **高级阴影**：点光源/聚光灯阴影、Cascaded Shadow Maps。
- **LOD系统**：细节层次管理。
- **GPU实例化**：高效渲染多个相同对象。
- **异步资源加载**：多线程资源加载，避免阻塞。
- **ECS架构**：实体组件系统，提高场景管理效率。
- **编辑器增强**：实时参数调整、资产浏览器、场景序列化。
- **调试工具**：渲染调试、性能分析、错误日志。

### 实现方案
1. **PBR渲染**：
   - 方案：扩展G-Buffer存储金属度/粗糙度，更新光照Pass使用Cook-Torrance BRDF。添加IBL（图像-based lighting）。
   - 步骤：修改着色器、添加新uniform、更新材质系统。

2. **后处理效果**：
   - 方案：创建PostProcessPass，应用全屏Quad渲染效果。支持链式后处理（多个Pass）。
   - 步骤：新增PostProcessPass类，实现Bloom/ToneMapping着色器。

3. **动画系统**：
   - 方案：集成Assimp加载动画数据，实现骨骼动画。添加AnimationComponent到ECS。
   - 步骤：添加动画类、更新Model类支持骨骼、着色器支持骨骼变换。

4. **物理集成**：
   - 方案：集成Bullet Physics，添加碰撞形状、刚体。同步物理与渲染。
   - 步骤：添加PhysicsSystem，更新Scene包含物理世界。

5. **高级阴影**：
   - 方案：扩展ShadowPass支持Omni/Spot阴影，使用Cubemap/Frustum。
   - 步骤：修改阴影着色器、添加多阴影贴图。

6. **LOD系统**：
   - 方案：Model存储多LOD网格，根据距离切换。
   - 步骤：添加LODComponent，视锥剔除时选择LOD。

7. **GPU实例化**：
   - 方案：使用glDrawArraysInstanced，传递实例数据。
   - 步骤：修改渲染调用，添加实例缓冲。

8. **异步资源加载**：
   - 方案：使用std::async或线程池加载资源，缓存到队列。
   - 步骤：添加ResourceLoader类，多线程加载。

9. **ECS架构**：
   - 方案：引入EnTT库，重构Scene为Registry。组件：Transform、Mesh、Light等。
   - 步骤：添加EnTT，迁移实体管理。

10. **编辑器增强**：
    - 方案：扩展ImGui界面，添加属性面板、序列化（JSON）。
    - 步骤：更新Editor类，添加UI控件。

11. **调试工具**：
    - 方案：添加渲染调试Pass、性能计数器、日志增强。
    - 步骤：实现DebugSystem，集成ImGui调试窗口。

### 优先级和计划
- 高优先级：PBR渲染、后处理效果（提升视觉质量）。
- 中优先级：异步加载、ECS（提升性能和扩展性）。
- 低优先级：动画、物理（特定需求）。

根据TODO逐步实现，从PBR开始。