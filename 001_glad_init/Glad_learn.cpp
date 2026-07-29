#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

//一般而言大写开头 == 类型，小写前缀 == 函数，全大写 == 常量
int main()
{
	//初始化窗口
	if (!glfwInit())
	{
		std::cout << "GLFW库初始化失败！" << std::endl;
		system("pause");
		return -1;
	}
	//限制glfw版本是3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设置OpenGL为核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口，判断是否创建成功
	GLFWwindow* window = glfwCreateWindow(800, 600, "LeatnOpenGl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

/*gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)
* ① glfwGetProcAddress
* 系统底层 OpenGL 函数地址查询工具：
* 显卡里几十上百个绘图函数（glClear、glDrawArrays等）没有直接固定地址，
* 需要运行时动态查询。
* 这个 GLFW 函数的作用：根据函数名字，拿到对应 OpenGL 函数的内存地址。
* 
* glad：GLAD 库名称（OpenGL Loader 库）
* Load：动词，加载、载入
* GL：Graphics Library，图形库（代指 OpenGL）
* Loader：名词，加载器
* 直译：GLAD 加载 OpenGL 加载器
* 
* ② GLADloadproc
* 强制类型转换：把 glfwGetProcAddress 转换成 GLAD 规定的标准函数指针格式，
* 保证 GLAD 能正常调用它。
* 
* proc = process /procedure 缩写，函数过程、函数指针
* 直译：GLAD 专用加载回调函数指针类型
* 
* ③ gladLoadGLLoader()
* GLAD 库的核心加载函数：
* 接收一个「查询 OpenGL 函数地址的工具」，自动遍历所有 OpenGL 3.3 函数，
* 挨个拿到地址、存入 GLAD 内部全局函数指针。
* 
* Get：获取
* Proc = procedure 函数过程
* Address：内存地址
* 直译：GLFW 获取 OpenGL 函数内存地址
* 
* 
* 整行逻辑
* 1，先拿到窗口上下文 glfwMakeContextCurrent(window)，必须先执行这一步；
* 2，把 GLFW 自带的地址查询函数交给 GLAD；
* 3，GLAD 自动加载全部 OpenGL 函数；
* 4，返回 true = 加载成功，false = 失败
*/

/*
* gladLoadGLLoader 这个函数需要接收一个函数指针作为参数
* glfwGetProcAddress 是一个函数名，单独写代表「这个函数的内存地址」（函数的指针）
* (GLADloadproc) 是强制类型转换，把 GLFW 的函数指针转换成 GLAD 需要的标准加载器指针类型
*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	return 0;
}
