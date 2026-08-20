#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
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
		std::cout << "Failed to initialize" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "my_first_triangle", NULL, NULL);
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

	/*
	* 一个顶点着色器可以声明多个不同location属性
	*/
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
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
		std::cout << "ERROR::SHADER::COMPILE::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"		FragColor = vec4(0.0f, 0.5f, 0.0f, 0.5f);\n"
		"}\n";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;

	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/*
	* 一个VAO可以记录多个不同的VBO
	* 
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	// location 0：绑定位置数据，来源 vbo_pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ... , (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	// location 1：绑定颜色数据，来源 vbo_color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, ... , (void*)0);
	glEnableVertexAttribArray(1);

	*VAO主要记住的是VBO的读取方法，和通道编号
	*/

	/*
	* 如果你在 VAO 激活期间，重新绑定另一个 EBO 到GL_ELEMENT_ARRAY_BUFFER，
	* VAO 会覆盖旧 EBO 记录，只记住最新的那一个。
	* EBO（GL_ELEMENT_ARRAY_BUFFER）是绑定在 VAO 状态内部的。
	* 一个 VAO 要么绑定了 EBO（走索引绘制 glDrawElements），
	* 要么没有 EBO（只能顺序绘制 glDrawArrays），
	* 不能 “同一个 VAO，一部分绘制用索引、一部分不用索引”。
	*/
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	float vertices[] =
	{
		 0.5f,  0.5f, 0.0f,   // 右上角，索引0
		 0.5f, -0.5f, 0.0f,  // 右下角，索引1
		-0.5f, -0.5f, 0.0f, // 左下角，索引2
		-0.5f,  0.5f, 0.0f   // 左上角，索引3
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	
	/*
	* 如果我们想绘制一个矩形，我们就需要绘制两个三角形，两个三角形需要六个顶点，
	* 这其中有两组重复的顶点，这会导致额外的开销，所以这时候我们使用元素缓冲对象，
	* 这时定义四个不同的顶点，通过EBO记录绘制顺序来实现四个顶点绘制两个三角形
	*/
	unsigned int indices[] =
	{
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形

		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processKey(window);
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* glDrawArrays 原理（顺序绘制）
		* 不使用 EBO，完全按照 VBO 里顶点存放的先后顺序读取顶点。
		* glDrawArrays(图元类型, 起始顶点下标, 一共读取多少个顶点);
		* 短板：
		* 想要绘制由两个三角形拼成的矩形，VBO 里必须写满6 个顶点（存在重复顶点），无法复用顶点。
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		*/
		/*
		* 依赖当前绑定的 EBO（索引缓冲），不按 VBO 顺序读取。
		* 参数：
		* GL_TRIANGLES：每 3 个索引构成一个三角形
		* 6：一共读取6 个索引值
		* GL_UNSIGNED_INT：EBO 里索引的数据类型是 unsigned int
		* 0：在这块 EBO 内存里，从第多少个字节开始读取索引。
		* 如果是（void*）4就是从第四个字节开始读取，这里就是跳过0直接读取1
		* （unsigned int一个元素占4字节）
		* 这里从0开始读取：此时EBO里有{0，1，3，1，2，3}索引从0到5，这里从0开始读取
		*/
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}
	/*清理资源前尽量：先解绑定，避免部分老旧驱动出现异常。*/
	glBindVertexArray(0);       // 解绑VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteVertexArrays(1, &VAO);//真正销毁释放 VAO 资源，程序结束前一定要做。
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	return 0;
}