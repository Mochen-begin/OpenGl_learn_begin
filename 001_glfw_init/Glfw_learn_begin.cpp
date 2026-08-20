// OpenGl_learn_begin.cpp: 定义应用程序的入口点。
//
/*
* GLFW：全部以 glfw 开头
* 类型：GLFWwindow GLFWmonitor GLFWcursor
* 函数：glfwCreateWindow glfwMakeContextCurrent glfwTerminate
* OpenGL 原生函数：全部以 gl 开头
* glClear glDrawArrays glShaderSource
*/
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

//一般而言大写开头 == 类型，小写前缀 == 函数，全大写 == 常量

int main()
{
/*glfwInit();
* glfwInit() = 初始化整个 GLFW 库
* GLFW 是第三方窗口库，它要调用 Windows 系统底层窗口、输入、显卡接口，
* 必须先执行这个函数完成内部初始化，分配底层资源。
* 1，连接操作系统窗口 API（Windows 就是 user32/gdi32）；
* 2，初始化鼠标、键盘、显示器输入检测功能；
* 3，探测本机显卡支持的 OpenGL 版本；
* 4，内部创建 GLFW 全局状态，为后面创建窗口做准备。
* 
* 返回 GLFW_TRUE（1）：初始化成功；
* 返回 GLFW_FALSE（0）：初始化失败（GLFW 库没链接、系统不支持等）
* 
* init = initialize 初始化
*/
// 初始化GLFW，判断是否成功
	if (!glfwInit())
	{
		std::cout << "GLFW库初始化失败！" << std::endl;
		return -1;
	}


/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
* glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
* 提前给即将创建的窗口设置配置要求，限制主版本号和副版本号
* 
* Hint：提示、预设配置
*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

/*glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
* glfwWindowHint( 配置项名称, 你要设置的值 );
* 找到「OpenGL 运行模式」这个设置项，把它的值改成「核心模式」
* 第一个参数是要修改的值，第二个参数是要改成的内容
* 告诉显卡：
* 我创建的窗口，只启用 OpenGL 3.3 核心模式，禁用所有老式固定管线 API。
* 
* 
* profile:个人资料、主页档案
* core profile:核心规范集
*/
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


/*GLFWwindow
* GLFWwindow 是 GLFW 库定义的不透明结构体
* 行业规则：所有操作窗口的逻辑，只能用指针 GLFWwindow*，不能直接创建 GLFWwindow win; 实体对象。
* GLFW 底层这个结构体存着一整套窗口相关数据：
* 1，窗口宽高、标题、全屏状态、窗口坐标；
* 2，当前绑定的 OpenGL 渲染上下文（最重要，OpenGL 绘图全靠它）；
* 3，鼠标、键盘输入状态缓存；
* 4，关联的监视器、光标资源；
* 5，窗口事件回调函数（点击、缩放、按键触发的函数）。
*/

/*glfwCreatewindow
* GLFWwindow* glfwCreateWindow(				//函数的声明
    int width,                // 参数1
    int height,               // 参数2
    const char* title,        // 参数3
    GLFWmonitor* monitor,     // 参数4
    GLFWwindow* share         // 参数5
);
*
* 参数1：横向像素宽度，就是有几个像素宽
* 参数2：竖向像素宽度，就是有几个像素高
* 参数3：该窗口的名字，窗口顶部标题栏文字，UTF-8 字符串
* 参数4：全屏监视器指针（新手固定填NULL）
* 参数5：资源共享窗口指针（新手固定填NULL）
*/

/*glfwTerminate();//不需要传参数(terminate：动词，含义是「终止、结束、清理、注销」)
* 释放 GLFW 全局所有占用的系统资源，是程序退出前必须调用的收尾函数。
* 它会做这些事：
* 1，销毁所有你创建过的 GLFWwindow 窗口；
* 2，释放鼠标光标、显示器、OpenGL 上下文等资源；
* 3，断开 GLFW 和操作系统窗口系统的连接；
* 4，清空内部全局状态、事件缓存。
*/
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

/*glfwMakeContextCurrent(window);
* (将窗口设置为当前 OpenGL 上下文（行业通用叫法，无官方中文名）
* 拆解单词：
* Make：设置、使成为
* Context：OpenGL 上下文
* Current：当前正在使用的
* 把你传入的 GLFWwindow* window 绑定为全局当前绘图窗口，
* 之后所有 OpenGL 绘图代码，都会渲染到这个窗口上。
* 必须是 glfwCreateWindow 成功创建、非 NULL 的窗口指针；
* 传入 NULL 会取消当前所有激活的上下文，之后无法绘图
* 
* 有唯一形参GLFWwindow*类型

*/
	glfwMakeContextCurrent(window);
	return 0;
}
