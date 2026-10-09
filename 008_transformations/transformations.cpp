#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#include<iostream>
#include<vector>
#include"Shader.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processKey(GLFWwindow* window);
int main()
{
	if (!glfwInit())
	{
		std::cout << "ERROR::INITIALIZE::GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl_transformations", NULL, NULL);

	if (window==nullptr)
	{
		std::cout << "FAILED::CREATE::WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::LOAD::GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);

	//顶点着色器
	int success;
	char infoLog[512];
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = 
		R"(
			#version 330 core
			layout ( location = 0 ) in vec3 aPos;
			layout ( location = 1 ) in vec3 aColor;
			layout ( location = 2 ) in vec2 aTexCoord;

			out vec3 ourColor;
			out vec2 ourTexCoord;

			void main()
			{
				gl_Position = vec4(aPos,1.0);
				ourColor = aColor;
				ourTexCoord = aTexCoord;
			}

		)";
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::\t" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	//片元着色器
	const char* fragmentShaderSource = 
		R"(
			#version 330 core
			out vec4 FragColor;

			in vec3 ourColor;
			in vec2 ourTexCoord;

			uniform sampler2D texture1;
			uniform sampler2D texture2;
			void main()
			{
				FragColor = vec4(ourColor,1.0);
			}
		)";
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMNET::\t" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	//渲染程序
	unsigned int program0 = glCreateProgram();
	glAttachShader(program0, vertexShader);
	glAttachShader(program0, fragmentShader);
	glLinkProgram(program0);
	glGetProgramiv(program0, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program0, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK::\t" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//纹理1
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_S)



	//VAO1,VBO1,EBO1
	unsigned int VAO1, VBO1, EBO1;
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);
	float vertices1[] =//矩形
	{
		-0.5f,0.5f,1.0f,   1.0f,0.0f,0.0f,  0.0f,1.0f,//左上
		0.5f,0.5f,1.0f,    0.0f,1.0f,0.0f,  1.0f,1.0f,//右上
		0.5f,-0.5f,1.0f,   0.0f,0.0f,1.0f,  1.0f,0.0f,//右下
		-0.5f,-0.5f,1.0f,  0.0f,1.0f,0.0f,  0.0f,0.0f//左下
	};
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);
	unsigned int indices1[] =
	{
		0,1,3,
		1,2,3
	};
	glGenBuffers(1, &EBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processKey(window);
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program0);
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
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