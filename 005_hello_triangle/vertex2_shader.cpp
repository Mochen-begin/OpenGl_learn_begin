#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
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
		std::cout << "Failed to initialize glfw" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	/*
	* ① 事件不会丢失
	* 窗口刚创建完成瞬间，系统可能立刻触发一次窗口大小事件（比如高分屏适配、窗口自动居中缩放）。
	* 先注册回调：窗口一创建，任何尺寸变化都会立刻触发回调，自动执行glViewport适配；
	* 后注册回调：窗口创建到绑定上下文这段时间的缩放事件会直接漏掉，可能出现画面拉伸、视口错位。
	* ② 代码分层清晰
	* 标准书写分层逻辑：
	* 创建窗口
	* 配置窗口所有回调（尺寸、鼠标、键盘）→ 纯窗口配置
	* 绑定 OpenGL 上下文 → 开启渲染相关功能
	* 把「窗口基础配置」和「OpenGL 渲染环境初始化」两段代码拆开，阅读更清晰。
	* ③ 统一所有回调的书写习惯
	* 以后你还要用到鼠标移动、按键输入、鼠标点击回调，全部遵循这套写法：
	* glfwCreateWindow → 立刻glfwSetXXXCallback注册 → 再glfwMakeContextCurrent
	* 统一格式，不用记特殊情况。
	*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	/*
	* framebuffer_size_callback 是GLFW 窗口系统的回调，只监听窗口缩放事件，
	* 属于 GLFW 窗口层逻辑；
	* 而 glfwMakeContextCurrent(window) 只是把 OpenGL 渲染上下文绑定到当前窗口，
	* 负责管理 GLAD、glViewport、VBO 这类 OpenGL 绘图接口。
	* 两者互不依赖：
	*/
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);

	/*
	* C++ 和 GLSL 是两套完全不同的语言，
	* 你现在直接把#version 330 core、layout(...)写在main()函数代码中，
	* 编译器会把它当成 C++ 语法，直接报红报错。
	* #verison 330 core
		layout(location = 0)in vec3 aPos;
	* 正确处理方式:
	* GLSL 代码必须放进 C++ 字符串，作为文本传给 OpenGL 驱动编译，不能直接裸写。
	*/
	
	/*
	* 储存字符串的时候，一般不用string因为OpenGL接口只认C风格的const char*
	* 如果用string类型的字符串需要用.c_str()转化成const char*类型
	*/

	/*顶点着色器含义：
	* 左边c++部分：
	* const：这段着色器文字全程不修改，只读
	* char*：c/c++存放字符串的标准类型
	* 
	* c++里多行连续双引号会自动拼接成一整段字符串；末尾加\n等价于GLSL的换行
	* 
	* #verion 330 core：
	* 声明着色器语言版本，和窗口设置的OpenGl3.3核心模式对应，必须放在着色器的最开头
	* 
	* layout (location = 0) in vec3 aPos：  【【layout：布局，规划，分配位置】】
	* layout：布局修饰符，用来给这个输入变量编号（通道号）
	* location=0：给这个顶点属性分配编号0，就是0号通道
	* 作用：后面的C++代码读取顶点数据时，要指定编号0才能和这个变量对接上
	* 它是语法修饰关键字，专门修饰变量 / 输入输出的绑定规则；
	* 它的写法 layout(xxx) 里的括号不是函数调用，是语法自带的固定格式；
	* 不能单独拿出来调用，只能写在变量声明最前面。
	* 
	* in：
	* GLSL变量修饰符，代表数据是从CPU输入进着色器
	* 数据流向：C++里写的三角形坐标->GPU顶点着色器->存入aPos变量
	* in：另一个修饰关键字；
	* 
	* vec3:
	* GLSL内置数据类型，三维浮点向量，自带x,y,z三个小数分量
	* 刚好匹配自定义的vertices[]每一组的三个数字：x,y,z
	* 
	* aPos：
	* 自定义变量名
	* a = attribute（顶点属性，CPU 传进来的数据）
	* Pos = Position（位置）
	* 你也可以改名比如vertexPos，只是约定俗成写 aPos。
	* 
	* vec3 aPos：变量类型 + 变量名
	* 
	* 
	* 整句合起来翻译：
	* 编号为 0 的输入变量，是一个三维向量，名字叫 aPos，数据从 CPU 传入着色器。
	* layout(location=0)：给变量分配 0 号通道编号（GLSL 专属绑定规则）
	* in：规定数据是外部输入进来的
	* vec3：规定变量存储数据的类型
	* aPos：变量名字
	*/

	/*
	* void main()：着色器入口函数，GPU 每一个顶点都会执行一次这个函数
	* gl_Position = vec4(...)：把 aPos 的三维坐标转为四维齐次坐标，
	* 我们将gl_Position设置的值会成为该顶点着色器的输出，必须写。
	* 
	*  gl_Position：内置全局变量名，不是类型，不能自定义
	* 规则：顶点着色器main函数必须给 gl_Position 赋值，否则 GPU 不知道顶点最终坐标，渲染直接报错黑屏。
	* 作用：存储这个顶点经过变换后的裁剪空间齐次坐标，光栅器会读取这个值生成三角形。
	* 命名规律：GLSL 自带的内置变量都以gl_开头，比如gl_FragCoord、gl_FragColor。
	* 
	*  vec4 () 是构造函数，不是普通函数，用来生成四维向量
	* vec4(参数1,参数2,参数3,参数4) 是 GLSL 提供的向量构造器，
	* 接收 4 个浮点数，打包成一个vec4类型四维向量。
	*/
	/* 编写顶点着色器源码 */
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"		gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
		"};"
		;
	/*
	* 为了能够让OpenGL使用它，我们必须在运行时动态编译它的源代码。
	* 我们首先要做的是创建一个着色器对象，注意还是用ID来引用的
	* 把需要创建的着色器类型以参数形式提供给glCreateShader。
	* 由于我们正在创建一个顶点着色器，传递的参数是GL_VERTEX_SHADER。
	*/
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	/*glShaderSourec:
	* glShaderSource函数把要编译的着色器对象作为第一个参数。
	* 第二参数指定了传递的源码字符串数量，这里只有一个。
	* 第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL。
	*/

	/*glCompileShader(vetrexShader)
	* OpenGL 编译着色器
	* Compile：动词，编译
	* 函数整体释义: glCompileShader(着色器ID)
	* 
	*/
	/*这几段代码运行逻辑：
	* glCreateShader(GL_VERTEX_SHADER)：向显卡申请一块空间，
	* 生成一个独一无二的数字 ID（vertexShader 就是这个数字）
	* 
	* glShaderSource(vertexShader, 1, &字符串, nullptr)：
	* 把你写好的 GLSL 文本代码，塞到这个 ID 对应的显卡内存里
	* 
	* glCompileShader (vertexShader)：核心作用
	* 拿显卡内存里存放的 GLSL 文本代码，进行编译。
	*/
	/*
	*  具体干了哪几件事
	* 语法检查：检测你 GLSL 有没有写错，比如少分号、layout 写错、变量未定义；
	* 类型检查：vec3、vec4 使用是否规范；
	* 生成 GPU 二进制指令：把人类写的着色器代码，转换成显卡硬件可执行的程序；
	* 编译结果存在这个 shader ID 对应的对象里，后面链接程序要用。
	* 3. 关键区分：只编译，不链接
	* 这个函数只单独编译顶点着色器 / 片段着色器，不会把多个着色器合并到一起。
	* 编译完之后，还需要：
	* glLinkProgram() 把编译好的顶点、片段着色器拼接成完整可运行的渲染程序。
	*/
	/* 我们把这个着色器源码附加到着色器对象上，然后编译它：*/
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	/*检测glCompileShader是否编译成功*/

	/*
	* 首先我们定义一个整型变量来表示是否成功编译，【整形success】
	* 只有两种值：1（编译成功） / 0（编译失败）
	* 还定义了一个储存错误消息（如果有的话）的容器。【容器infoLog[512]】【log：日志】
	* 【info = information 信息】【infoLog 直译：信息日志（报错日志）】
	* 如果编译报错，显卡会把错误原因、报错行数、语法问题这段文字存进这个数组里；
	* 没报错里面就是空的。
	* 512 是足够存下大部分报错信息的长度，初学直接抄就行。
	* 然后我们用glGetShaderiv检查是否编译成功。
	* 
	* 函数：glGetShaderiv
	* 单词拆分记忆
	* gl：OpenGL 函数前缀
	* Get：获取
	* Shader：着色器
	* i：int 整数
	* v：vector / 数组，代表参数是指针（内存地址）
	* 直译：获取着色器的整型状态
	* 
	* 三个参数逐个说：
	* vertexShader：你之前创建的顶点着色器 ID（数字），告诉函数：我要查这个着色器
	* GL_COMPILE_STATUS：固定枚举值，含义：查询「编译状态」【status：状态】
	* &success：取 success 变量的内存地址
	* OpenGL 会把查询结果（1 成功 / 0 失败）写到这个地址对应的 success 变量里
	*/
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);

	/*如果编译错误就打印错误日志*/
	/*glGetShaderInfoLog()
	* gl：OpenGL 前缀
	* Get：获取
	* Shader：着色器
	* Info = information 信息
	* Log：日志、记录
	* 直译：获取着色器信息日志
	* 函数一共有四个参数：
	* vertexShader
	* 你要查询的着色器 ID（顶点着色器编号）。告诉 OpenGL：我想要读取这个着色器的日志。
	* 
	* 512 → maxLength
	* 最大能读取多少个字符。
	* 你的 char infoLog[512] 数组最多装 512 个字符，这里填 512，
	* 防止文字过长造成内存越界。
	* 
	* NULL → length 输出参数
	* 作用：接收「实际读到的日志字符串长度」。
	* 初学我们暂时不需要知道精确长度，直接填NULL，表示忽略这个返回值。
	* 
	* infoLog
	* 字符数组，内存容器。
	OpenGL 找到报错信息后，把错误文本拷贝进这个数组，
	* 之后你就可以 cout 打印查看哪里 GLSL 写错了。
	*/
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog 
			<< std::endl;
		glfwTerminate();
		return -1;
	}

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);


		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
	/*glDeleteBuffers(1, &VBO);
	* n：要销毁多少个缓冲 ID
	* buffers：缓冲 ID 数组，单个 VBO 就传&VBO
	* 根据传入的 VBO 编号，查询驱动内部映射表
	* 找到这个 ID 绑定的那块连续显存区块。
	* 释放 GPU 显存
	* 显卡内存管理器回收这块显存，这块空间可以分配给后续新缓冲、纹理使用，不再占用显存。
	* 作废该缓冲 ID
	* 这个数字 ID 永久失效，后续不能再用它执行glBindBuffer、glBufferData等操作。
	* 清空驱动内该 ID 对应的资源记录
	* 一般程序结束以后会自动删除所有占用的内存，包括显存的缓冲，但是大型项目，比如游戏，
	* 一直在运行会导致显存泄露，显存会被一点一点占满，后面会闪退或者花屏
	*/
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}