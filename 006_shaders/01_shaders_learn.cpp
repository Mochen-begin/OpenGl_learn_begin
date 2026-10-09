#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
void framebuffer_size_callback(GLFWwindow*window,int width,int height)
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

	//着色器的结构：
	"#version version_number"//版本version
	"in type in_variable_name;"//输入类型in
	"in type in_variable_name;"//输入类型in
	"out type out_variable_name;"//输出类型out
	"uniform type uniform_name;"
	"void main()"
	"{"
	// 处理输入并进行一些图形操作
	//...
	// 输出处理过的结果到输出变量
	"      out_variable_name = weird_stuff_we_processed;"
	"}";

	/*glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&nrAttributes)
	* 翻译：获取整数向量值
	* gl：OpenGL 前缀
	* Get：获取
	* Integer：整数
	* v：vector 向量（代表参数是指针，可以返回一组 / 一个数值到变量里）
	* 记忆口诀：gl-Get-Integer-v  OpenGL 获取整数 (返回至变量指针)
	* ATTRIBS = Attributes：属性（顶点属性：位置、颜色、法线、纹理坐标这类输入到顶点着色器的数据）
	* 
	* 函数原型简化理解:
	* void glGetIntegerv(GLenum pname, GLint *params);
	* pname：查询项的枚举编号，你要查什么参数
	* params：指针，OpenGL 会把查询出来的整数，写到你传进去的这个变量地址
	* 作用：读取 OpenGL 状态机里的整数值，结果通过指针输出到你的变量，函数本身没有返回值。
	*/
	int nrAttributes;//顶点属性能储存的上限
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout <<"Maximum nr of vertex attributes supported: "<< nrAttributes 
		<< std::endl;


	/*GLSL
	* 全称：OpenGL Shading Language
	* 包含基础数据类型：int,float,double,uint和bool
	* 包含容器类型两个：向量（Vector),矩阵（Matrix）
	* 
	* 
	* 向量：可以包含有2，3或者4个分量的容器，分量类型可以是上面提到的任意数据类型
	* 
	* （下面的n是数字，代表着该向量的分量个数）
	* 类型	含义
	* vecn	包含n个float分量的--默认向量
	* bvecn	包含n个bool分量的向量
	* ivecn	包含n个int分量的向量
	* uvecn	包含n个unsigned int分量的向量
	* dvecn	包含n个double分量的向量
	* 
	* 大多数时间使用vecn，因为float足够满足大多数需求了
	* 
	* 向量的分量可以由.x  .y  .z  .w获取，
	* 对于颜色也允许用  .r   .g   .b   .a
	* 或是对纹理坐标使用.s  .t  .p  .q
	*/

	//向量的重组：
	"vex2   v1;"
	"vec4   v2=v1.xyxx;"
	"vec3   v3=v2.zyw;"
	"vec4   v4=v1.xxxx + v3.yxzy";

	//可以把一个向量作为一个参数传递给不同的向量构造函数
	"vec2 vect = vec2(0.5, 0.7);"
	"vec4 result = vec4(vect, 0.0, 0.0);"
	"vec4 otherResult = vec4(result.xyz, 1.0); ";



	/*
	* 每个着色器使用这两个关键字设定输入和输出（in,out），
	* 只要一个输出变量与下一个着色器阶段的输入匹配，
	* 它就会传递下去。但在顶点和片段着色器中会有点不同。
	* 
	* 顶点着色器：
	* 顶点着色器应该接收的是一种特殊形式的输入，否则就会效率低下。
	* 顶点着色器的输入特殊在，它从顶点数据中直接接收输入。
	* 为了定义顶点数据该如何管理，我们使用location这一 元数据 指定输入变量，
	* 这样我们才可以在CPU上配置顶点属性。
	* layout (location = 0)。顶点着色器需要为它的输入提供一个额外的layout标识，
	* 这样我们才能把它链接到顶点数据。
	* 
	* 片段着色器：
	* 一个例外是片段着色器，它需要一个vec4颜色输出变量，
	* 因为片段着色器需要生成一个最终输出的颜色。
	* 如果你在片段着色器没有定义输出颜色，OpenGL会把你的物体渲染为黑色（或白色）。
	* 
	* 所以，如果我们打算从一个着色器向另一个着色器发送数据，
	* 我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入。
	* 当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起，
	* 它们之间就能发送数据了（这是在链接程序对象时完成的）。
	*/
	unsigned int VertexShader;
	const char* VertexShaderSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"//位置变量的属性位置值为0
		"out vec4 vertexColor;\n"//为片段着色器指定一个颜色输出
		"void main()\n"
		"{\n"
		"		gl_Position = vec4(aPos,1.0);\n"//注意我们如何把一个vec3作为vec4的构造函数
		"		vertexColor = vec4(0.5,0.0,0.0,1.0);\n"	//把输出变量设置为暗红色
		"}\n";
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderSource,NULL);
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

	unsigned int FragmentShader;
	const char* FragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec4 vertexColor;\n"//从顶点着色器传来输入变量（名称相同，类型相同）
		"void main()\n"
		"{\n"
		"		FragColor = vertexColor;\n"
		"};\n";
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShader(FragmentShader);
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILE::\n" << infoLog << std::endl;
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

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}
	return 0;
}