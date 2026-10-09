#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
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
		std::cout << "Failed to initialize glfw" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn_OpenGL", NULL, NULL);
	if (window == nullptr)
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

	unsigned int VertexShader;
	const char* VertexShaderSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"		gl_Position = vec4(aPos,1.0);\n"
		"}\n";
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(VertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILE::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}


	/*Uniform: 【形容词】统一的，一致的
	* 
	* uniform是修饰关键字，后面跟类型，再跟变量名。
	* uniform是一种CPU向GPU传递数据的一种形式
	* 1---在同一个program内部uniform变量全局可见，顶点着色器，片段着色器等可以读取同一个uniform变量
	* 不同的program之间uniform相互独立，互不影响
	* 
	* 2---同一个program里uniform类型的变量名字必须唯一，uniform类型可以设置多个
	* 错误示例：
	* 顶点着色器：
	glsl
	uniform vec3 lightColor;
	* 片段着色器：
	 glsl
	 uniform vec4 lightColor; // 同一个program，重名 lightColor，链接失败！
	* 哪怕类型不一样，只要名字lightColor相同，就冲突。
	* 
	* 正确示例（多个不同名字 uniform）
	* 顶点着色器
	* glsl
	* uniform mat4 model;
	* uniform mat4 view;
	* 
	* 3---状态保持
	* 一旦 CPU 给 uniform 设置了数值，这个值会一直保存在这个着色器程序里，
	* 直到 CPU 代码再次修改它。不会随着绘制调用自动重置。
	*/
	unsigned int FragmentShader;
	const char* FragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor;\n"//这里没有赋值！
								  //GLSL 里面不能写uniform vec4 ourColor = vec4(1,0,0,1);
								  //uniform 的值必须由 CPU 侧 OpenGL 代码传入。
		"void main()\n"
		"{\n"
		"		FragColor = ourColor;\n"// 真正读取使用了 ourColor
		"};\n";
	/*
	* 着色器代码实际读取了ourColor的值。
	* 链接进 Program 之后，ourColor会保留在 program 的 uniform 符号表里。
	* CPU 侧 glGetUniformLocation(program, "ourColor") 可以拿到有效的 location（不是 -1），
	* 可以正常赋值。
	*/
	/*注意！！！！！！！！！
	* 如果没有使用，只保留了uniform的声明：
	* 单个 shader 编译 (compile) 阶段不会删掉它；
	* 等到 glLinkProgram【链接整个 program】的时候，会做全局死代码消除，
	* 把未被使用的 uniform 剔除。
	* 
	* 规则：只要在该 program 下，任意一个着色器（顶点 / 片段 / 几何等）真正读取这个 uniform，
	* 它就保留；全部都不读取才会链接时优化清除。
	*/
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShader(FragmentShader);
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::COMPILE::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}


	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, VertexShader);
	glAttachShader(shaderProgram, FragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROFILE::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	

	glBindVertexArray(0);

	
	while (!glfwWindowShouldClose(window))
	{
		processKey(window);
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		/*
		* 1,忘记 glUseProgram：glGetUniformLocation和glUniform*
		* 都依赖当前激活的 program，不 useProgram，赋值无效。
		* 
		* 2,uniform 在 shader 里定义了但是没有实际使用：
		* GLSL 编译器会做优化直接删掉这个 uniform，glGetUniformLocation返回 - 1。
		* 
		* 3,uniform 名字大小写敏感，C++ 代码字符串名字必须和 GLSL 完全一致。
		* 
		* 4,uniform 只能 CPU-->GPU 单向传递，着色器内部不能修改 uniform 的值，
		* shader 代码只能读，不能写。修改只能回到 CPU 调用 glUniform 接口。
		*/
		glUseProgram(shaderProgram);
		
		/*glfwGetTime()
		* GLFW 初始化之后流逝的秒数，返回类型是double双精度浮点数，精度很高。
		* 当你调用glfwInit()初始化 GLFW 成功那一刻，计时器就从零开始跑。
		* 返回值：程序运行的总秒数，可以带小数，例如 2.345，代表 GLFW 已经运行了 2.345 秒。
		* 不受帧率影响，真实物理时间。哪怕游戏卡顿，这个时间依旧按照现实时间往前走。
		*/
		float timeValue = glfwGetTime();
		float color = fabs(sin(timeValue));

		/*glGetUniformLocation(shaderProgram,"ourColor")
		* 参数:
		* 1,program：链接完成的着色器程序对象 ID
		* 2,name：字符串，就是 GLSL 代码里 uniform 变量的名字，大小写严格区分。
		* 
		* 返回值：（有符号整数）
		* >=0：成功，返回这个 uniform 的位置索引 (location)。
		* 后面调用glUniform*系列函数就要用这个编号，用来告诉 OpenGL：我要修改哪一个 uniform。
		* -1：找不到这个 uniform。
		* 注意：location 不是数组下标，不保证一定从 0 开始，
		* 不要硬写死数字，运行时必须调用这个函数去查询。
		* 
		* 重点：这个函数不会报错，不会触发 OpenGL 错误，安静返回 - 1。
		* 如果你不去判断返回值，继续把-1丢进glUniform4f,
		* OpenGL 会静默忽略调用，画面没有任何效果，调试很难受。
		* 
		* 调用glGetUniformLocation不需要提前 glUseProgram (shaderProgram)！
		* 查询 location 只是读取 program 对象内部信息，不需要激活使用。
		*/
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		/*glUniform*
		* uniform的类型。可能的后缀有：
		* 后缀	含义
		* f	    函数需要一个float作为它的值
		* i	    函数需要一个int作为它的值
		* ui	函数需要一个unsigned int作为它的值
		* 3f	函数需要3个float作为它的值
		* fv	函数需要一个float向量/数组作为它的值
		* 
		* glUniform4f(参数1，参数2，参数3，参数4，参数5)
		* 参数1：要改的uniform类型的地址
		* 参数2~5：赋值给uniform类型变量的数值
		*/
		glUniform4f(vertexColorLocation, 0.1f, color, 0.1f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
	}
	return 0;
}