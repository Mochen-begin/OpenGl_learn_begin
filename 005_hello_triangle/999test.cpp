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
	const char* fragmentShaderSource1 =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"		FragColor = vec4(0.0f, 0.5f, 0.0f, 1.0f);\n"
		"}\n";
	unsigned int fragmentShader1;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader1);
	glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	unsigned int shaderProgram1;
	shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;

	}
	glDeleteShader(fragmentShader1);


	const char* fragmentShaderSource2 =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"		FragColor = vec4(0.0f, 0.0f, 0.5f, 1.0f);\n"
		"}\n";
	unsigned int fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;

	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader2);


	unsigned int VAO1;
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);
	float vertices1[] =
	{
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	unsigned int VBO1;
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	float vertices2[] =
	{
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 1.0f,  0.0f, 0.0f
	};
	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processKey(window);
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glUseProgram(0);这个函数是直接覆盖，不需要先关闭
		glUseProgram(shaderProgram2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));


		
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0,3);
		glfwSwapBuffers(window);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
	return 0;
}