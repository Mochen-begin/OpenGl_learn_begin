# 1. 规定当前项目需要的最低CMake版本
cmake_minimum_required (VERSION 3.20)

# 2. 设置项目总名称（仅标识文件夹，不影响生成的exe程序名）
project ("Opengl_learn_begin")

# 3. 配置C++编译标准：强制使用C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 关闭编译器扩展，严格遵循标准C++语法
set(CMAKE_CXX_EXTENSIONS OFF)

# ====================== 第三方库路径配置（只需要改这里） ======================
# GLFW窗口库安装根目录
set(GLFW_ROOT "E:/OpenGL_Libs/glfw_install")
# GLAD OpenGL函数加载库安装根目录
set(GLAD_ROOT "E:/OpenGL_Libs/glad_install")

# ====================== 头文件搜索路径 ======================
# 告诉编译器去哪里找 #include <glad.h> / #include <GLFW/glfw3.h>
include_directories(
    ${GLFW_ROOT}/include
    ${GLAD_ROOT}/include
)

# ====================== 静态库搜索路径 ======================
# 告诉链接器去哪里找 glfw3.lib 库文件
link_directories(${GLFW_ROOT}/lib)

# ====================== GLAD固定编译文件 ======================
# glad.c 必须参与编译，所有程序都要带上它，单独提取出来复用
set(GLAD_SRC ${GLAD_ROOT}/src/glad.c)


# ====================== 程序1：001_glad_init GLAD单独初始化测试 ======================
add_executable(001_glad_init
    001_glad_init/Glad_learn.cpp
    ${GLAD_SRC}
)
# 和上面链接一模一样的依赖库
target_link_libraries(001_glad_init PRIVATE
    glfw3
    opengl32.lib
    user32.lib
    gdi32.lib
    shell32.lib
)

# ====================== 程序2：002_glfw_init 窗口初始化示例 ======================
# add_executable(生成的exe名称 源代码文件列表)
add_executable(002_glfw_init
    # 你的代码文件路径
    002_glfw_init/Glfw_learn_begin.cpp
    # 固定带上glad源码
    ${GLAD_SRC}
)
# 给窗口程序链接所有依赖库
# PRIVATE：仅当前程序内部使用这些库，不对外暴露
target_link_libraries(002_glfw_init PRIVATE
    glfw3          # GLFW窗口库
    opengl32.lib   # Windows系统自带OpenGL驱动库
    user32.lib     # Windows窗口系统必备依赖
    gdi32.lib      # Windows图形界面依赖
    shell32.lib    # Windows窗口交互依赖
)

#=========================程序3：003_viewport 视口===================================
add_executable(003_viewport
    003_viewport/Viewport.cpp
    ${GLAD_SRC}
)
target_link_libraries(003_viewport PRIVATE
    glfw3
    opengl32.lib 
    user32.lib 
    gdi32.lib 
    shell32.lib 
)
#==========================程序4：004_render_loop 渲染循环=============================
add_executable(004_render_loop
  004_render_loop/Render_loop.cpp 
  ${GLAD_SRC}
)
target_link_libraries(004_render_loop PRIVATE
    glfw3 
    opengl32.lib 
    user32.lib 
    gdi32.lib 
    shell32.lib 
)