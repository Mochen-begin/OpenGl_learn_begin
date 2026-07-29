#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
///////////////////////////////////////////////////
// 如果先写GLFW再写glad会出错：
//1，glfw3.h 内部自带系统 opengl 头文件；
//2，glad.h 也完整定义了一套 OpenGL；
//两者重复定义大量 GL_XXX 宏、函数、类型，GLAD 为了防止冲突，直接主动抛出这个编译错误阻止编译。
// 
//原理：
//先包含 glad.h：GLAD 会定义一个保护标记，标记 “OpenGL 定义已由 GLAD 提供”；
//后包含 glfw3.h：GLFW 检测到已有 GLAD 提供 OpenGL，就不会再加载系统原生 gl 头文件，冲突消失。
// 
//额外注意事项：
//整个项目任何地方都不要手动引入 <GL / gl.h> / <GL / glcorearb.h>，GLAD 全权替代；
//所有 cpp 文件统一遵循：先 glad.h，再 glfw3.h，再其他头文件；
//如果其他源文件也同时包含这两个头，全部统一修改 include 顺序。
/////////////////////////////////////////////////
/*
* framebuffer：帧缓冲//callback：回调函数//frame：帧 
* 用来修正视口大小和位置的，只要用户拖动窗口缩放窗口，代码手动修改窗口大小GLFW都会自动调用这个函数
* 程序刚启动的时候不会主动调用这个
*/

//自定义函数，名字写成这个是为了见名知意//修正视口尺寸大小
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
int main()
{
	if (!glfwInit())
	{
		std::cout << "GLFW窗口初始化失败！" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600,"Learn_OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to Creat GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	/*Viewport 视口
* 是OpenGL 渲染专用绘图区域，窗体内部一块矩形画布，所有三角形、纹理、图形只会画在这块区域里。
* 作用：把 OpenGL 标准设备坐标 [-1,1]（标准化设备坐标 NDC）映射到屏幕像素坐标。
* 完全独立于窗体大小：可以比窗口小、偏移、只占窗口一部分。
*
* ////注意的点：
* 1，窗口缩放时必须手动回调更新，否则图形拉伸变形
* 2，视口大小可以超过窗口大小，但是超过的部分不显示，就是不会渲染，看不到，会导致画面被截断
*/
/*glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
*					GLFW 设置帧缓冲尺寸回调
*
* 把你写好的 framebuffer_size_callback 这个函数注册给窗口对象 window
* 相当于告诉 GLFW：
* 以后只要这个窗口大小发生变化，你自动去调用 framebuffer_size_callback 里面的代码
*
* 注意放置位置：
* 1，必须在 glfwCreateWindow 创建窗口之后：第一个参数是 window，窗口都没创建会空指针崩溃
* 2，建议放在渲染循环之前，只要窗口存在，随时监听缩放事件
*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//视口回溯要紧跟在窗口创建上下文后面，不用写在视口初始化后面


/*glViewport
* 前两个参数是视口左下角的坐标值，后两个参数是视口的横向大小和竖向大小，就是宽和高，
* 就是后面的参数才是高度
* 
*/

	glViewport(0, 0, 800, 600);

	glfwTerminate(); // 新增，正常退出也要销毁窗口、释放GLFW
	return 0;
}