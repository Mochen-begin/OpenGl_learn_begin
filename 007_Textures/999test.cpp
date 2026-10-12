#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#include"Shader.h"
float myMix = 0.5f;
float myCoordx = 0.0f,myCoordy=0.0f;
void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
	glViewport(0,0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		myMix += 0.001f;
		if (myMix >= 1.0f)myMix = 1.0f;
		std::cout <<"myMix = "<< myMix << std::endl;
		myCoordy += 0.001f;
		if (myCoordy >= 1.5f)myCoordy = 1.5f;

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		myMix -= 0.001f;
		if (myMix <= 0.0f) myMix = 0.0f;
		std::cout << "myMix = " << myMix << std::endl;
		myCoordy -= 0.001f;
		if (myCoordy <= -1.5f)myCoordy = -1.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		myCoordx -= 0.0005f;
		if (myCoordx <= -1.5f)myCoordx= -1.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		myCoordx += 0.0005f;
		if (myCoordx >= 1.5f)myCoordx = 1.5f;
	}
}
int main()
{
	//初始化glfw
	if (!glfwInit())
	{
		std::cout << "ERROR::INITIALIZE::GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "ERROR::CREATE::WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}
	//设置回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);
	//load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::LOAD::LOADER" << std::endl;
		glfwTerminate();
		return 0;
	}
	glViewport(0, 0, 800, 600);

	//顶点着色器
	const char *vertexShaderSource = 
		R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aColor;
		layout (location = 2) in vec2 aTexCoord;
		uniform float myCoordx;
		uniform float myCoordy;
		out vec3 ourColor;
		out vec2 ourTexCoord;

		void main()
		{
			gl_Position = vec4(aPos.x+myCoordx,aPos.y+myCoordy,aPos.z,1.0);
			ourColor = aColor;
			ourTexCoord = aTexCoord;
		}
		)";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	int success;
	char infoLog[512];
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::VERTEX::SHADER\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	//片元着色器
	const char* fragmentShaderSource = R"(
		#version 330 core
		in vec3 ourColor;
		in vec2 ourTexCoord;
		uniform float myCoord;
		uniform sampler2D texture1;
		uniform sampler2D texture2;
		uniform float mytest;
		out vec4 Color;
		void main()
		{
			Color = mix(texture(texture1,ourTexCoord),texture(texture2,ourTexCoord),mytest);
		}
		)";
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::SHADER\t" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	//项目
	unsigned int myProgram = glCreateProgram();
	glAttachShader(myProgram, vertexShader);
	glAttachShader(myProgram, fragmentShader);
	glLinkProgram(myProgram);
	glGetProgramiv(myProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(myProgram, 512, NULL, infoLog);
		std::cout << "ERROR::LINK::PROGRAM\t" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//纹理1
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);//防止上下颠倒
	unsigned char* data1 = stbi_load("E:/OpenGl_learn/OpenGl_learn_begin/textures/container.jpg",&width,&height,&nrChannels,0);
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::GENERATE::TEXTURE1" << std::endl;
		glfwTerminate();
		return -1;
	}
	stbi_image_free(data1);

	//纹理2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	float color1[] = { 0.3f,0.2f,0.2f,1.0f };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);//防止上下颠倒
	unsigned char* data2 = stbi_load("E:/OpenGl_learn/OpenGl_learn_begin/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::GENERATE::TEXTURE2" << std::endl;
		glfwTerminate();
		return -1;
	}
	//VAO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VBO
	glGenBuffers(1, &VBO);
	const float vertices[] =
	{
		-0.5f,-0.5f,1.0f,   1.0f,0.0f,0.0f, 0.0f,0.0f,
		-0.5f,0.5f,1.0f,    0.0f,1.0f,0.0f, 0.0f,1.0f,
		0.5f,0.5f,1.0f,     0.0f,0.0f,1.0f, 1.0f,1.0f,
		0.5f,-0.5f,1.0f,    1.0f,1.0f,0.0f, 1.0f,0.0f

	};
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);
	//EBO
	unsigned int indices[] =
	{
		0,1,2,
		0,2,3
	};
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);

	unsigned int VAO2, VBO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	const float vertices2[] =
	{
		-0.5f,-0.5f,1.0f,  1.0f,0.0f,0.0f,
		0.0f,0.5f,1.0f,    0.0f,1.0f,0.0f,
		0.5f,-0.5f,1.0f,   0.0f,0.0f,1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	////确定纹理通道
	//glUseProgram(myProgram);
	//glUniform1i(glGetUniformLocation(myProgram, "texture1"), 0);
	//glUniform1i(glGetUniformLocation(myProgram, "texture2"), 1);

	glUseProgram(myProgram);
	glUniform1i(glGetUniformLocation(myProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(myProgram, "texture2"), 1);
	////调用自己的Shader
	//Shader program("E:/OpenGl_learn/OpenGl_learn_begin/shader.vs", "E:/OpenGl_learn/OpenGl_learn_begin/shader.fs","#define USE_TEXTURE");
	//program.use();
	//program.setInt("texture1", 0);
	//program.setInt("texture2", 1);
	//program.setFloat("mixVale", 0.5);
	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		/*Shader program2("E:/OpenGl_learn/OpenGl_learn_begin/shader.vs", "E:/OpenGl_learn/OpenGl_learn_begin/shader.fs");
		program2.use();*/
		/*unsigned int locUnifo = glGetUniformLocation(myProgram, "myCoord");
		float time = glfwGetTime();
		float color = sin(time);
		glUniform1f(locUnifo, color);*/
		glUseProgram(myProgram);
		glUniform1f(glGetUniformLocation(myProgram, "mytest"), myMix);
		glUniform1f(glGetUniformLocation(myProgram, "myCoordx"), myCoordx);
		glUniform1f(glGetUniformLocation(myProgram, "myCoordy"), myCoordy);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		/*glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);*/
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
	}
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	glfwTerminate();
	return 0;
}