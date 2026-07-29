#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/*glfwGetKey()
* GLFW获取按键状态
* 参数1：目标窗口
* 参数2：要查询按键常量GLFW_KEY_ESCAPE=键盘ESC键
* 返回两种固定枚举：
* GLFW_PRESS:按键当前处于按住状态
* GLFW_RELEASE:按键松开，未按下
*/

/*glfwSetWindowShouldClose(参数1，参数2)  //和渲染循环的函数不一样，这是个set类的函数
* 1,参数 1：window —— 指定要修改哪一个窗口
* 2,参数 2：value —— 你要把标记改成什么值
* 
* GLFW 设置窗口为应当关闭状态
*/

//把ESC作为退出程序的按键
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn_OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad " << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glViewport(0, 0, 800, 600);
	//渲染循环
/*glfwWindowShouldClose(window)
* 在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话，
* 该函数返回true，渲染循环将停止运行，之后我们就可以关闭应用程序
*/

/*glfwSwapBuffers(window)//swap:交换
* 交换当前窗口的前后缓冲区：
* 把画好内容的后缓冲送到屏幕（变成前缓冲，显示器刷新出来）；
* 原来显示过的旧前缓冲，变成空白后缓冲，供下一帧绘图使用。
*/

/*
* 双缓冲(Double Buffer)
* 应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。
* 这是因为生成的图像不是一下子被绘制出来的
* 而是按照从左到右，由上而下逐像素地绘制而成的。最终图像不是在瞬间显示给用户，
* 而是通过一步一步生成的，这会导致渲染的结果很不真实。为了规避这些问题，
* 我们应用双缓冲渲染窗口应用程序。前缓冲保存着最终输出的图像，它会在屏幕上显示；
* 而所有的的渲染指令都会在后缓冲上绘制。当所有的渲染指令执行完毕后，
* 我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。
*/

/*glfwPollEvents()
* glfw：这个函数属于 GLFW 窗口库
* Poll：轮询、主动查询（主动去系统拿消息）
* Events：事件（窗口操作、按键、鼠标等用户交互行为）
* 直译：GLFW 轮询所有事件
* 
* 作用：
* 主动向操作系统查询、读取当前堆积的所有窗口 / 输入事件，并自动执行你提前注册好的回调函数。
* 
* 会处理哪些事件
* 1，窗口类：拖动窗口、缩放窗口、点击关闭按钮、最小化 / 还原窗口
* 2，输入类：键盘按下 / 松开、鼠标移动、鼠标左键右键点击
* 配套行为：
* 1，检测到窗口缩放 → 自动调用 framebuffer_size_callback
* 2，点击窗口关闭叉号 → 给窗口打上关闭标记，glfwWindowShouldClose() 就会返回 true
*/


/*GLFW执行逻辑：
* GLFW 不会在后台偷偷监听用户操作，只有执行这一行代码才会处理事件；
* 一次性清空当前系统消息队列里所有积压事件，处理完立刻返回，不阻塞程序；
* 每一帧渲染循环都要调用一次。
*/

	//标准工业级正确顺序（输入→事件→渲染→交换缓冲）
	while (!glfwWindowShouldClose(window))
	{
		         //输入

		//检测按下的按键是否为ESC
		processInput(window);

		glfwPollEvents();//先执行本循环的所有操作

		         //渲染指令

		/*glClearColor()
		* 直译：设置清空画布所用的背景颜色
		* 四个参数：红、绿、蓝、透明度，取值范围固定 0.0 ~ 1.0
		* 0.0：最低（黑色 / 完全透明）
		* 1.0：最高（纯色 / 完全不透明）
		* 单独写这一行不会清空屏幕，只是提前规定好 “等下清空时用什么颜色填充”；
		*/

		/*glClear(GL_COLOR_BUFFER_BIT)  直译：清空颜色缓冲区
		* 
		* GL_COLOR_BUFFER_BIT：颜色缓冲区标记
		* 是 OpenGL 内置常量，代表「只清理存放画面颜色的缓冲区」。
		* OpenGL 不止颜色缓冲区，还有深度缓冲区、模板缓冲区，
		* 这里我们现阶段只需要清理屏幕画面，只用这个标记。
		* 
		* 缓冲位(Buffer Bit)
		* 
		* 作用：
		* 把当前后缓冲整个画布，用之前 glClearColor() 设置好的底色全部填充覆盖，
		* 擦掉上一帧所有画面内容，防止画面残影、拖影。
		*/
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		
		glfwSwapBuffers(window);//后交换两个缓冲
		
	}

	glfwTerminate();
	return 0;
}