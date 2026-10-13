# OpenGL 学习记录

跟学 [LearnOpenGL CN](https://learnopengl-cn.github.io/) 的代码记录，一个 CMake 工程，每个章节编译为独立可执行文件。

## 项目结构

| 目录 | 对应内容 |
|---|---|
| `001_glfw_init` | GLFW 初始化 |
| `002_glad_init` | GLAD 初始化 |
| `003_viewport` | 视口与着色器、纹理初步 |
| `004_render_loop` | 渲染循环 |
| `005_hello_triangle` | Hello Triangle |
| `006_shaders` | 着色器 |
| `007_Textures` | 纹理 |
| `008_transformations` | 变换 |
| `009_CoordinateSystems` | 坐标系统 |
| `999review` | **合上教程后从零重写**，用来检验是否真正理解，而非复制粘贴 |

> `999review` 是本仓库最重要的部分。跟教程敲一遍只能证明“看懂了”，从零重写才能证明“会了”。

## 编译方式

```bash
cmake -B build
cmake --build build
