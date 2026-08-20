#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
	glViewport(0, 0, width, height);
}
void processKey(GLFWwindow* window)
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);

	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"		gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
		"}\n";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog 
			<< std::endl;
	}

	/*
	* 片段着色器(Fragment Shader)是第二个也是最后一个我们打算创建的用于渲染三角形的着色器。
	* 片段着色器所做的是计算像素最后的颜色输出。
	* 
	* 在计算机图形中颜色被表示为有4个元素的数组：
	* 红色、绿色、蓝色和alpha(透明度)分量，通常缩写为RGBA。
	* 当在OpenGL或GLSL中定义一个颜色的时候，我们把颜色每个分量的强度设置在0.0到1.0之间。
	* 比如说我们设置红为1.0f，绿为1.0f，我们会得到两个颜色的混合色，即黄色。
	* 这三种颜色分量的不同调配可以生成超过1600万种不同的颜色！
	*/

	/*OpenGl和GLSL
	* OpenGL 与 GLSL 清晰区分（适配你 LearnOpenGL 当前学习阶段）
	* 1. OpenGL
	* 全称：Open Graphics Library
	* 本质：一套 CPU 端 C 语言风格 API（函数库标准）
	* 运行位置：CPU
	* 作用：CPU 给 GPU 下达指令，管理 GPU 资源
	* 能干的事情：
	* 创建缓冲区、创建着色器对象、上传数据、切换管线、发起绘制、窗口交互。
	* 通俗比喻：OpenGL = 指挥（CPU 上的指挥官），负责发号施令。
	* 2. GLSL
	* 全称：OpenGL Shading Language OpenGL 着色器语言
	* 本质：运行在 GPU 上的编程语言
	* 运行位置：GPU（着色器核心）
	* 作用：编写着色器逻辑（顶点着色器、片段着色器等）
	*/

	/*
	* out：输出变量修饰符。
	* 片段着色器必须定义至少一个 out 变量。
	* 含义：每一个像素最终输出的颜色。
	* vec4 四分量向量：(红，绿，蓝，透明度)，值域通常 \([0,1]\)。
	* GPU 会自动把这个颜色写入帧缓冲，显示到屏幕。
	* 对比顶点着色器：顶点着色器用 in 接收顶点数据；片段着色器用 out 输出像素颜色。
	* 这里的FragColor是自定义变量名，和main里的FragColor一样，可以任意设置
	*/
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(0.0f,0.0f,0.5f,0.5f);\n"
		"}\n";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_FRAGMENT_SHADER, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog 
			<< std::endl;
	}


	/*
	* 着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。
	* 如果要使用刚才编译的着色器我们必须把它们链接(Link)为一个着色器程序对象，
	* 然后在渲染对象的时候激活这个着色器程序。
	* 已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。
	* 当链接着色器至一个程序的时候，它会把每个着色器的输出链接到下个着色器的输入。
	* 当输出和输入不匹配的时候，你会得到一个连接错误。
	*/

	/*
	* glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。
	*/
	unsigned int shaderProgram;//用来储存程序对象id
	shaderProgram = glCreateProgram();

	/*glAttachShader  (attach:附加，挂载）
	* program：着色器程序对象 ID（由glCreateProgram()创建）
	* shader：编译完成的着色器对象 ID（顶点着色器 / 片段着色器等）
	* 作用:
	* 将一个已经编译完成的着色器对象挂载（附加）到着色器程序上。
	* 着色器程序本身是空容器，必须把顶点着色器、片段着色器依次 attach 进来，
	* 后续调用glLinkProgram时，驱动才能把多个着色器拼接成一条完整 GPU 渲染管线。
	* 
	* 添加顺序不会影响最后链接结果
	* 原理拆分：
	* glAttachShader 仅仅是把着色器对象登记到 Program 的内部列表里，
	* 只是单纯保存引用，不执行任何拼接、编译、校验。
	* 真正检查「是否同时具备顶点着色器、片段着色器」、组装渲染管线的操作，
	* 发生在后续 glLinkProgram。
	* 重要规则:
	* 只是建立关联，attach 操作不会复制着色器代码；
	* 一个 program 可以 attach 多个着色器，
	* 但同一阶段着色器只能有一份（不能两个顶点着色器同时附加）；
	* attach 之后依然可以修改原着色器源码，必须重新编译 + 链接才生效；
	* 链接完成后，可以使用glDeleteShader删除着色器对象，
	* 只要还附加在 program 上，资源不会立刻销毁。
	*/
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	/*
	* 函数作用
	* 遍历所有通过 glAttachShader 挂载到 program 的着色器，由显卡驱动完成下面一系列工作：
	* 检查各个着色器之间输入、输出变量是否匹配（比如顶点着色器 out 和片段着色器 in 类型要对应）；
	* 将多个阶段着色器组装成一条完整可用的 GPU 渲染管线；
	* 解析 uniform 变量、分配硬件资源，生成 GPU 能够执行的最终程序。
	* 重点区分：
	* glCompileShader：编译单个着色器（顶点 / 片段着色器单独编译）
	* glLinkProgram：链接多个着色器，拼成完整管线
	*/
	/*
	* 你目前只提供：顶点着色器 + 片段着色器
	* glAttachShader 挂上两者
	* glLinkProgram：只连接【顶点着色器 <---> 片段着色器】之间的数据通路
	* GPU 运行时自动插入内置固定阶段：
	* 顶点着色输出 -> 图元装配 -> 光栅化 -> 送入片段着色器
	* 如果你后续想要启用几何着色器：
	* 必须手动创建几何着色器 -> attach 到 program -> 再执行 glLinkProgram。
	* 不 attach，管线里就跳过几何着色器，不会自动生成。
	*/

	/*
	* 可编程部分（你可以写 GLSL 控制，需要 attach+link）
	* 顶点着色器（必选，必须提供）
	* 几何着色器（可选，不写就自动不存在）
	* 片段着色器（必选，必须提供）
	* 
	* 固定功能阶段（硬件内置，不受 glLinkProgram 控制，永远存在）
	* 顶点着色器输出 
	* 图元装配 
	* 光栅化 
	* 片段着色器输入
	*/
	glLinkProgram(shaderProgram);

	/*
	* 我们也可以检测链接着色器程序是否失败，并获取相应的日志。
	*/
	//Status：状态
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog
			<< std::endl;
	}

	/*
	* glUseProgram函数，用刚创建的程序对象作为它的参数，以激活这个程序对象：
	* 必须链接成功的 program 才能使用；链接失败的 program 调用后无法正常渲染。
	* OpenGL 是状态机：一旦调用生效，之后持续使用这个 program，
	* 直到再次调用glUseProgram切换为另一个程序。
	* 调用时机：绘制之前执行。
	* 多个着色器程序切换绘制不同物体时，在每组绘制前调用该函数切换管线。
	*/
	/*
	* 完整流程位置回顾
	* 创建着色器-->编译 --> attach --> glLinkProgram --> glUseProgram --> 发起绘制
	*/
	glUseProgram(shaderProgram);
	/*
	* 调用 glDeleteShader 只是发起 “删除请求”，不是立刻销毁内存。
	* OpenGL 使用引用计数机制：
	* 着色器对象还被 program 通过 attach 关联着 -> 引用计数 > 0
	* 即使你马上调用 glDeleteShader，资源不会立刻释放，只是标记待删除
	* 等到两种情况才真正销毁：
	* 该着色器从 program 上 detach；
	* 对应的 program 被 glDeleteProgram 删除。
	* 结论：链接完成后立刻写 glDeleteShader 是标准规范写法，完全安全。
	*/
	/*引用计数：
	* OpenGL 对象采用引用计数：每有一处持有这个对象，计数 +1；
	* glDeleteShader 不会立刻销毁，作用：
	* 把对象打上「待删除标记」；
	* 当引用计数降到 0 + 带有待删除标记 -> 真正销毁释放内存。
	* 
	* 
	* 
	unsigned int vertexShader = glCreateShader(...); // 引用计数 = 1
	glAttachShader(program, vertexShader);            // program额外引用它 → 引用计数 = 2

	glLinkProgram(program);
	glDeleteShader(vertexShader);
	执行 glDeleteShader 之后：
	引用计数 不会立刻减少，仅仅标记：没人再打算使用这个句柄。
	此时计数依旧 = 2。
	*/
	/*
	* 为什么官方推荐立刻删除？
	* 防止手误保留无用句柄，避免内存泄漏；
	* 着色器源码、编译工作全部完成，后续再也不需要单独操作着色器对象；
	* 养成良好习惯，不用等到程序末尾统一清理。
	*/

	/*标记删除以后的情况：
	* 1，标记删除后，句柄还能用吗？
	* 可以正常使用
	* 只要对象没真正销毁，你依然可以调用：
	* glGetShaderiv(vertShader, GL_COMPILE_STATUS, ...)
	* 查询信息不会报错，功能正常。
	* 因为内存还没释放。
	* 2， 还能再次 glAttachShader 挂载到另一个 program 吗？
	*  不行！
	* 一旦调用 glDeleteShader 打上待删除标记：
	* OpenGL 禁止再把这个着色器 attach 到任何新 Program。
	* 也就是：无法再产生新引用，引用计数不能继续增加。
	* 3， 什么时候彻底消失？
	* 两个条件同时满足：
	* 对象带有【待删除标记】（调用过 glDeleteShader）
	* 所有引用全部断开，引用计数 = 0
	* 此时驱动立刻释放资源。
	* 
	* VBO、Texture 这类缓冲区对象也是一模一样的规则
	*/
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{
		processKey(window);
		glfwPollEvents();
		glClearColor(0.3f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
	glDeleteBuffers(1, &VBO);
	return 0;
}