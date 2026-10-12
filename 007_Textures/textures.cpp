#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
		std::cout << "Faile to initialize glad" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);

	
	int success;
	char infoLog[512];
	const char* vertexShaderSource = 
		R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 aColor;
			layout (location = 2) in vec2 aTexCoord;
			out vec3 ourColor;
			out vec2 TexCoord;
			void main()
			{
				gl_Position = vec4(aPos,1.0f);
				ourColor = aColor;
				TexCoord = aTexCoord;
			}
		)";
		
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::VERTEXSHADER::COMPILE::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}

	/*
	* 片段着器也要访问纹理对象
	* GLSL有一个供纹理对象使用的内建数据类型，叫做采样器（Sampler）
	* 它以纹理类型作为后缀，比如sample1D、sample3D等
	*/
	/*GLSL: texture函数
	* 参1：纹理采样器
	* 参2：对应纹理坐标
	* 
	* texture函数会使用之前设置的纹理参数对应的颜色值进行采样，
	* 这个片段着色器输出的就是纹理的纹理坐标上的颜色
	*/
	/*GLSL:mix函数
	* mix函数需要接收两个值作为参数，并根据第三个值进行混合，
	* 第三个值就是第二个参数的占比
	* 如果第三个值是0.0就会返回第一个输入，如果第三个值是1.0就会返回第二个输入，
	* 0.2就会返回80%第一个输入和20%第二个输入的混合色
	*/
	const char* fragmentShaderSource =
		R"(
			#version 330 core
			out vec4 FragColor;

			in vec3 ourColor;
			in vec2 TexCoord;

			uniform sampler2D texture1;
			uniform sampler2D texture2;

			void main()
			{
							//texture(texture2, vec2(1.0-ourTexCoord.x,ourTexCoord.y)水平镜像反转是吧坐标映射到1->0,不能用x坐标乘-1的方法，这是映射到0 -> -1是否镜像反转和环绕方式有关
				FragColor = mix(texture(texture1, TexCoord) , texture(texture2,TexCoord),0.5);
			}
		 )";

		
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::SHADER::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	unsigned int program;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK::\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
	* 为了能够把纹理映射(Map)到三角形上，我们需要指定三角形的每个顶点各自对应纹理的哪个部分。
	* 这样每个顶点就会关联着一个纹理坐标(Texture Coordinate)，
	* 用来标明该从纹理图像的哪个部分采样（译注：采集片段颜色）。
	* 之后在图形的其它片段上进行片段插值(Fragment Interpolation)。
	*
	* 纹理坐标在x和y轴上，范围为0到1之间（注意我们使用的是2D纹理图像）。
	* 使用纹理坐标获取纹理颜色叫做采样(Sampling)。
	*/

	/*纹理的环绕方式：
	* GL_REPEAT               对纹理的默认行为，重复纹理图像
	* GL_MIRRORED_REPEAT      和GL_REPEAT一样，但每次重复图片是镜像放置的
	* GL_CLAMP_TO_EDGE        纹理会被约束在0，1之间，超出部分会重复纹理边缘，//`CLAMP` 的本意：**钳制、夹住**。把超出 `[0,1]` 的纹理坐标，强行 “钉死” 在区间的边界上。edge = **边缘、边界**
	*                         产生一种边缘被拉伸的效果。
	* GL_CLAMP_TO_BORDER      超出的坐标为用户指定的边缘颜色//border（边框）
	*
	*
	*
	* 可以使用使用glTexParameter*函数对单独的一个坐标轴设置   //Parameter参数，
	* （s、t（如果是使用3D纹理那么还有一个r）它们和x、y、z是等价的）：
	* 如果用最后一个类型就要单独传一个颜色值
	* float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	* glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	*
	*/
	/*glTexParameter*  //Tex:texture 纹理  ，Parameter参数，
	*  i是int的缩写，代表这个函数最后一个参数是整形
	* 
	* 参数1：GL_TEXTURE_2D
	* 表明纹理目标，这个代表我们正在配置2D纹理
	* 
	* 参数2：GL_TEXTURE_WRAP_S//wrap 动词，包裹，环绕
	* 要修改的纹理属性，s代表纹理的u轴（就是水平方向坐标轴），对应纹理坐标（s,t）中的s
	* GL_TEXTURE_WRAP_S就是设置水平方向超出[0,1]的环绕行为，T代表v轴（竖直方向）
	* 
	* 参数3：GL_MIRRORED_REPEAT//mirrored：镜像的，静脉内反射的
	* repeat 重复，循环
	* 设置环绕模式
	*/
	/*设置纹理环绕方式*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//如果选择GL_CLAMP_TO_BORDER就要用fv后缀模式,就是float类型的数组（vector)
	//这里的第二个参数BORDER:border：边框，边界
	//先设置纹理环绕方式为GL_CLAMP_TO_BORDER然后再设置边框颜色
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//纹理边框颜色
	float borderColor[] = { 1.0f,1.0f,0.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	//纹理的过滤：
	/*
	* 纹理坐标不依赖于分辨率(Resolution)，它可以是任意浮点值，
	* 所以OpenGL需要知道怎样将纹理像素(Texture Pixel，也叫Texel)映射到纹理坐标。
	* 
	* 纹理过滤有很多个选项，我们目前只讨论最重要的两组：
	* GL_NEAREST 邻近过滤
	* OpenGL默认过滤方式，会选择中心点最接近纹理坐标的那个像素
	* 做出来的图片是8bit风格，清晰但是一块一块的
	* 
	* GL_LINEAR线性过滤
	* 基于纹理坐标附近的纹理像素，计算出一个插值，近似出这些纹理像素之间的颜色
	* 看起来比较平滑，但是模糊
	*/

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	* 参数1：纹理类型
	* 参数2：修改纹理的哪一个属性，写的是属性名字
	*	MIN = Minify，缩小。物体变小、屏幕像素少于纹素，**纹理缩小采样过滤器**
	*	MAG：Magnify，放大（纹理被放大显示在屏幕上的时候）
	*	FILTER：过滤器，采样算法
	* 参数3：修改成的方式
	* 
	*/
	// //设置纹理过滤方式也用glTexParameter*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	/*多级渐远纹理
	* 当纹理分辨率很高时，但是要贴图的物体却很远，这时就会导致采样点之间间隔过大，导致采样
	* 时没法正确采样到纹理的像素点，导致不真实性，而且使用高分辨率纹理本身也会浪费内存，
	* 这个时候多级渐远纹理（Mipmap）就派上用场了
	* 
	* 过滤方式	描述
	* GL_NEAREST_MIPMAP_NEAREST	
						使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
	* GL_LINEAR_MIPMAP_NEAREST	
						使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
	* GL_NEAREST_MIPMAP_LINEAR	
				   在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
	* GL_LINEAR_MIPMAP_LINEAR	
						在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样

	* 同样使用glTexParatemer*来设置纹理过滤方式
	*/
	//只有远距离的物体，也就是minify的时候mipmap才有用，对magnify使用mipmap会报错
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	/*
	* width：图片**宽度（像素）**
	* height：图片**高度（像素）**
	* nrChannels：图片**通道数量**
	* JPG：一般是 3 通道 RGB（红、绿、蓝）
	* PNG：一般是 4 通道 RGBA（多一个透明通道 A）
	* 这三个变量是**输出参数**，`stbi_load` 内部会自动填充它们的值，所以传地址 `&width`。
	*/
	/*stbi_load//只是读取一个函数，纹理生成要专门调用函数
	* 参1：贴图地址
	* 参2：宽度
	* 参3：高度
	* 参4：通道数
	* 参5：- 这个参数告诉 stb_image：**你希望加载出来的像素内存，最终保留几个通道**
	- 取值可选：`0,1,2,3,4`
	  1. `0`：**不做强制转换，使用图片本身原生通道数**。图片是 3 通道 jpg，就输出 RGB（3 字节 / 像素）；如果是带透明的 png（4 通道 RGBA）就输出 4 通道。此时`nrChannels`会拿到图片原始通道数量。
	  2. `1`：强制转为灰度图（单通道）
	  3. `2`：灰度 + alpha
	  4. `3`：强制转为 RGB，丢弃透明通道（就算原图有 alpha 也扔掉）
	  5. `4`：强制转为 RGBA，没有 alpha 的图就自动补 255 不透明 alpha
	*/
	int width5, height5, nrChannels5;
	unsigned char* data5 = stbi_load("E:/OpenGl_learn/OpenGl_learn_begin/textures/container.jpg", &width5, &height5, &nrChannels5, 0);


	//生成纹理id

	/*
	* glGenTextures函数首先需要输入生成纹理的数量，
	* 然后按照生成的数量把id给后一个参数，如果一次生成大于一个，第二个参数就要是数组
	*/
	unsigned int texture5;
	glGenTextures(1, &texture5);

	//绑定纹理通道
	glBindTexture(GL_TEXTURE_2D, texture5);
	//生成纹理

	/*glTexImage2D//image 图像，图片
	* 函数作用：**把 CPU 内存里的像素数据，上传到当前绑定的 GPU 纹理对象，真正创建纹理图像**。
	* 参1：纹理目标类型（就是刚才绑定的那个）
	* 参2：mipmap层级，设置为0代表基本级别。（当你想自己设置的时候用这个）
	* 参3：要把纹理储存为何种形式
	* （当前的图片只有RGB值，所以这里用GL_RGB，如果还有深度值就用GL_RGBA（A:(Alpha 透明度)）)
	* 参4，5：图片宽度和高度
	* 参数6：统一写0这个是历史遗留问题
	* 参7：读取的图片在内存储存的格式（上个函数jpg读取出来是RGB，png读取出来是RGBA）
	* 参8：代表每一个颜色分量的数据类型（这里是unsigned char对应OpenGL就是GL_UNSIGNED_BYTE)
	* 在图形 API 的术语里：**1 Byte = 8 bit**就是一个字节
	* 参9：cpu端像素数组指针（就是上个函数读取的返回值真正储存读取过来的图片的东西）
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width5, height5, 0, GL_RGB, GL_UNSIGNED_BYTE, data5);
	//自动生成所有需要的多层渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data5);

	//正确的生成纹理顺序========================================================
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//设置环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//设置过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//加载并生成纹理
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);//防止上下颠倒//flip 翻转 vertically副词，垂直地、上下方向 on 在…… 之时 load 加载
	unsigned char* data1 = stbi_load("E:/OpenGl_learn/OpenGl_learn_begin/textures/container.jpg", &width, &height, &nrChannels,0);
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		glfwTerminate();
		return -1;
	}
	//用完记得把读取的图片数据删掉
	stbi_image_free(data1);
	//加载纹理2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	int width2, height2, nrChannels2;
	/*
	* OpenGL要求y轴0.0在图片底部，但是图片的y轴0.0坐标通常在顶部
	*/
	stbi_set_flip_vertically_on_load(true);//防止上下颠倒
	unsigned char* data2 = stbi_load("E:/OpenGl_learn/OpenGl_learn_begin/textures/awesomeface.png", &width2, &height2, &nrChannels2, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	//用完记得把读取的图片数据删掉
	stbi_image_free(data2);

	//生成顶点数组对象
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO;
	//应用纹理：
	const float vertices[] = 
	{
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	unsigned int indices[] =
	{
		0,1,3,
		3,1,2
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);


	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "texture1"), 0);
	glUniform1i(glGetUniformLocation(program, "texture2"), 1);

	int i = 0;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		/* 纹理单元 */

		/*
		* 使用glUniform1i可以给纹理采样器sampler分配一个位置值，这样就能在一个片段着色器中
		* 设置多个纹理。
		* 一个纹理的位置值通常被称为一个纹理单元，一个纹理的默认纹理单元是0，
		* 它是默认激活的纹理单元，所以教程之前没有分配一个位置值
		*/


		/*glActiveTexture(GL_TEXTURE0);
		* OpenGL至少保证有16个纹理单元供你使用，
		* 也就是说你可以激活从GL_TEXTURE0到GL_TEXTRUE15。
		* 它们都是按顺序定义的，
		* 所以我们也可以通过GL_TEXTURE0 + 8的方式获得GL_TEXTURE8，
		* 这在当我们需要循环一些纹理单元的时候会很有用。
		*
		* OpenGL 里有一组**纹理单元 (Texture Unit)**，相当于一组 “纹理插槽”。
		* glActiveTexture(xxx)` 的作用：**选中接下来要操作哪一个纹理插槽**。
		*/

		/*
		* 1. glActiveTexture(GL_TEXTURE0)：把全局当前活动纹理单元切到 0 号插槽。
		* 2. glBindTexture(target, texID)：
		* 在当前活动纹理单元内部，找到GL_TEXTURE_2D这个类型的绑定点，
		* 把texture（纹理对象）挂上去。
		*/
		glActiveTexture(GL_TEXTURE0);// 选中【纹理单元0】（插槽0）
		//这个active函数必须写在glBindTexture之前
		glBindTexture(GL_TEXTURE_2D, texture1);//作用就是吧
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glUseProgram(program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		std::cout << "帧" << i++ << std::endl;

		glfwSwapBuffers(window);
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(program);
	glfwTerminate();
	return 0;
}