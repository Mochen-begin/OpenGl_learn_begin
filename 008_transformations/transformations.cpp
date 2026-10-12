#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION//implementation执行、履行、完成
#include<stb_image.h>
#include<iostream>
#include<vector>
#include"Shader.h"
#include<glm/glm.hpp>//GLM核心主头文件，提供基础向量、矩阵类型（vec3、mat4等）
#include<glm/gtc/matrix_transform.hpp>//常用矩阵变换扩展 translate/rotate/scale等
#include<glm/gtc/type_ptr.hpp>
#include<cmath>
//类型指针工具，把glm矩阵/向量转成c风格float指针，方便传给OpenGl的glUniformMatrix4fv
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
			uniform mat4 transform;//GLSL里也有mat4 4*4方阵类型
			out vec3 ourColor;
			out vec2 ourTexCoord;

			void main()
			{
				gl_Position = transform * vec4(aPos,1.0);
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
				FragColor = mix(texture(texture1,ourTexCoord),texture(texture2,ourTexCoord),0.5);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannel;
	stbi_set_flip_vertically_on_load(true);//防止上下颠倒
	unsigned char* data1 = stbi_load("E:/OpenGl_learn/OpenGl_learn_begin/textures/container.jpg", &width, &height, &nrChannel, 0);
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to create texture1" << std::endl;
		glfwTerminate();
		return -1;
	}
	stbi_image_free(data1);

	//纹理2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data2 = stbi_load("E:/OpenGl_learn/OpenGl_learn_begin/textures/awesomeface.png", &width, &height, &nrChannel, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to create texture2" << std::endl;
		glfwTerminate();
		return -1;
	}
	stbi_image_free(data2);
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

	glUseProgram(program0);
	glUniform1i(glGetUniformLocation(program0, "texture1"), 0);
	glUniform1i(glGetUniformLocation(program0, "texture2"), 1);




	//变换：
	/*
	* 齐次坐标(Homogeneous Coordinates)
	* 向量的w分量也叫齐次坐标。
	* 想要从齐次向量得到3D向量，我们可以把x、y和z坐标分别除以w坐标。
	* 我们通常不会注意这个问题，因为w分量通常是1.0。
	* 使用齐次坐标有几点好处：它允许我们在3D向量上进行位移
	* （如果没有w分量我们是不能位移向量的），而且下一章我们会用w值创建3D视觉效果。
	* 如果一个向量的齐次坐标是0，这个坐标就是方向向量(Direction Vector)，
	* 因为w坐标是0，这个向量就不能位移（译注：这也就是我们说的不能位移一个方向）。
	*/
	/*变换的顺序
	* 缩放--旋转--位移//顺序尽量不要变，否则会导致出错
	* 比如，如果你先位移再缩放，位移的向量也会同样被缩放（译注：比如向某方向移动2米，2米也许会被缩放成1米）！
	*/

	//使用glm库的类型或者函数的时候直接加glm::作用域就行
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);//直接构造初始化

	//matrix /ˈmeɪtrɪks/矩阵
	//glm::mat4就是4*4方阵，直接初始化为glm::mat4 trans(1.0f)就是初始化为单位矩阵
	glm::mat4 trans2 = glm::mat4(1.0f);//拷贝构造
	glm::mat4 trans(1.0f);//我们用的是1.1.0版本，所以必须手动初始化矩阵，
	//直接写成glm::mat4 trans;不会默认初始化，直接用会导致未定义行为

	/*glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	* 返回一个和参数一同阶的方阵
	* 
	* 参数一：基础矩阵，相当于在这个现有变换的基础上叠加平移
	* 参数二：平移向量（x,y,z),代表要移动多少
	* 
	* 这里：
	* m 是单位矩阵 I，结果 = T x I = T，得到只做平移 (1,1,0) 的矩阵。
	* 作用：把顶点坐标 x+1，y+1，z 不变。
	* 
	*/
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	//矩阵要左乘向量
	vec = trans * vec;
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;



	//把箱子进行旋转
	glm::mat4 trans0(1.0f);

	/*glm::scale(trans0, glm::vec3(0.5, 0.5, 0.5));
	* 缩放矩阵，同样的会返回第一个矩阵同阶的矩阵
	* 参数一：基础矩阵//这个 m 必须是合法初始化的矩阵，不能是glm::mat4 trans0;这种未初始化脏矩阵。
	* 参数二：放大的倍数，必须是vec3，本别代表把x，y，z缩放多大倍数
	* 1. 缩放因子为 0：对应坐标轴直接压缩消失，顶点全部塌到原点。
	* 2. 缩放因子为负数：坐标轴镜像翻转，同时缩放大小。
	*/
	trans0 = glm::scale(trans0, glm::vec3(0.5, 0.5, 0.5));//trans0=trans0*vec3
	/*glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	* 旋转函数，和translate函数一样是在第一个矩阵的基础上左乘一个旋转矩阵
	* 生成的都是绕原点旋转的矩阵
	* 参数一：基础矩阵
	* 参数二：旋转的角度，要求弧度制（就是多少pi）
	* 参数三：围绕旋转的旋转轴向量
	* 1. 类型：必须是glm::vec3，不能是 vec2/vec4。
	* 2. 归一化：GLM 内部会自动归一化，不需要自己手动变成单位向量。
	* 但是工程习惯上，**建议直接写单位向量**，可读性更好，也省去 GLM 额外一次 sqrt 运算。
	* 
	* radians函数就是把角度制转换为弧度制的函数//radian 弧度制、弧度
	*/
	trans0 = glm::rotate(trans0, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	


	/*
	* 要用Matrix4fv为后缀的函数给uniform mat4赋值
	* glUniformMatrix4fv
	* 第一个参数是地址
	* 第二个是传递矩阵数量
	* 第三个询问是否需要转置矩阵（把行和列交换）
	* OpenGL开发者通常使用一种内部矩阵布局，叫做列主序(Column-major Ordering)布局。
	* GLM的默认布局就是列主序，所以并不需要转置矩阵，我们填GL_FALSE。
	* 第四个参数是真正的矩阵数据，但是glm的矩阵不是OpenGL要接受的那种类型所以要用
	* glm::value_ptr()函数转换 /value 值/ptr = pointer 指针
	*/

	//给顶点着色器的transform赋值
	glUniformMatrix4fv(glGetUniformLocation(program0, "transform"), 1, GL_FALSE, glm::value_ptr(trans0));
	
	Shader program("E:/OpenGl_learn/OpenGl_learn_begin/shader.vs", "E:/OpenGl_learn/OpenGl_learn_begin/shader.fs", "#define USE_TEXTURE");
	program.use();
	program.setInt("texture1", 0);
	program.setInt("texture2", 1);
	program.setFloat("mixValue", 0.5f);

	//把两个纹理绑定到不同的纹理单元//这个绑定是全局的，不用每一帧都绑定
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	while (!glfwWindowShouldClose(window))
	{
		/*
		* glfwPollEvents() 的作用：

			1. 去操作系统抓取窗口、键盘、鼠标事件；
			2. 更新内部保存的按键状态表；
			3. 触发回调函数（framebuffer_size_callback 这类）。

			glfwGetKey()只是读取 GLFW 内部已经缓存好的按键状态，它不会主动去操作系统查询按键
			所以
			
		processKey(window);
		glfwPollEvents();
			这样写会导致processKey处理的Key是上一帧的内容，会导致判断滞后一帧导致输入延迟

			正确逻辑：事件轮询 → 输入处理 → 更新逻辑 → 清除缓冲区 → 绘制 → 交换缓冲区（目前更新逻辑还没学到）
		*/
		glfwPollEvents();//事件轮询
		processKey(window);//输入处理
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);//清除缓冲区


		//实现让盒子随着时间旋转
		trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//这个结果是trans=trans*R,然后trans=trans*T结果就是为trans*R*T到了真正用就变成先平移后旋转了
		

		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));//后平移
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		glUseProgram(program0);
		glUniformMatrix4fv(glGetUniformLocation(program0, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		

		
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//第二个盒子
		program.use();
		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		trans = glm::scale(trans, glm::vec3(fabs(sin((float)glfwGetTime())), fabs(sin((float)glfwGetTime())), 1.0f));
		program.setMat4("transform", trans);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(sin((float)glfwGetTime()), sin((float)glfwGetTime()), 0.0f));
		program.setMat4("transform", trans);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//交换缓冲区
		glfwSwapBuffers(window);
	}
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO1);
	glDeleteVertexArrays(1, &VAO1);
	glDeleteProgram(program0);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
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