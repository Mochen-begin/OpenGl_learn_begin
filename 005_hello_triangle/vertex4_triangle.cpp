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
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(0.0f,0.0f,0.5f,1.0f);\n"
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
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog
			<< std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*顶点数组对象(Vertex Array Object, VAO)
	* 单词拆解：
	* Vertex：顶点
	* Array：数组
	* Object：对象
	* 核心一句话：
	* VAO 是一个大收纳盒，专门用来记录全部顶点属性的配置信息。
	* VAO 只记录：“我要去哪个 VBO 上读数据、怎么读”。
	* 它记录：
	* 各个glVertexAttribPointer设置的解析规则
	* 哪些属性通道被glEnableVertexAttribArray开启 / 关闭
	* 这些属性对应哪一个 VBO 缓冲区
	* 注意：VAO不储存顶点本身的数据，顶点数据仍然存放在 VBO 里面。VAO 只存 “说明书、配置表”
	* VAO 记录的是「去哪里读」，VBO 里面存的是「被读的原始内容」。
	* VBO 里面数据随便改，VAO 不用重新配置。
	* VAO 不知道 VBO 里面存的是什么数字，只知道读取规则。
	* 注意：不能换另外一个不同的 VBO 给这个旧 VAO。
	* glVertexAttribPointer执行那一刻，就会把当时绑定的 VBO 编号，直接写进 VAO 内部。
	* 后续就算你 bind 别的 VBO，这个 VAO 依然绑定当初那一个 VBO。
	* 如果想让 VAO 读取另一个 VBO，你必须：重新 bindVAO，重新调用一遍glVertexAttribPointer。
	* 
	* 工作流程：
	* glGenVertexArrays()：向 OpenGL 申请，拿到一个 VAO 编号
	* glBindVertexArray(vaoID)：绑定这个 VAO，把它变成当前正在使用的收纳盒
	* 关键点：从这一行绑定之后，你后面执行的
	* glVertexAttribPointer()、glEnableVertexAttribArray()
	* 全部都会记录进当前绑定的这个 VAO 里面。
	* 绑定 VBO、拷贝顶点数据、设置属性指针、启用通道。这些配置全部写入 VAO。
	* glBindVertexArray(0)：解绑 VAO，收纳盒合上。
	* 以后想要再次使用这套顶点配置，只需要一行：
	* glBindVertexArray(vaoID);
	* 所有属性指针、启用状态全部自动恢复，不需要重复写一大堆 glVertexAttribPointer。
	*/

	/*glGenVertexArrays(1, &VAO);
	* 第一个参数1：生成1 个VAO 对象
	* 第二个参数&VAO：传入你定义的 unsigned int 变量的地址，
	* OpenGL 会把生成出来的 VAO 编号写进这个变量。
	* 作用：
	* 向 OpenGL 驱动申请 1 个 VAO 对象，返回一个无符号整数 ID，存到变量VAO。
	* 注意：仅仅只是申请 ID 号，此时 VAO 对象还没有被激活使用，只是拿到它的身份证号码。
	* 类比：去物业登记，拿到一个房间号，但是房间还没有打开。
	*/
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	/*glBindVertexArray(VAO);
	* Bind：绑定
	* 作用：
	* 把这个 VAO 设置为当前正在激活的 VAO。
	* 从执行完这一行开始：
	* 之后执行的 glVertexAttribPointer、glEnableVertexAttribArray，
	* 所有顶点属性配置，全部记录进这个绑定的 VAO 内部。
	*/
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

	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	* gl：Graphics Library 图形库
	* Vertex：顶点
	* Attrib = Attribute：属性
	* Pointer：指针
	*
	* 函数作用
	* 告诉 GPU：如何从当前绑定的 VBO 中读取顶点属性数据
	* 建立「着色器内 layout(location = 0) in vec3 aPos」和缓冲区数据的映射关系。
	*
	* 0 -> 属性位置 location
	* 对应着色器 layout(location = 0)，两者数字必须一致。
	*
	* 3 -> 每个顶点属性由几个数值组成
	* vec3 坐标 x,y,z -> 3 个浮点数。
	*
	* 第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
	*
	* 第四个参数定义我们是否希望数据被标准化(Normalize)。
	* 如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。
	* 我们把它设置为GL_FALSE。
	*
	* 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。
	* 由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。
	* 要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）
	* 我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
	* 一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，
	* 我们在后面会看到更多的例子
	*
	* (void*)0 → 缓冲区偏移 offset
	* 从 VBO 内存起始位置开始读取；
	* 如果前面还有别的数据，就要填对应字节偏移。
	*
	* 调用此函数之前：
	* 必须 glBindBuffer (GL_ARRAY_BUFFER, VBO);
	* 该函数只会作用于当前绑定到 GL_ARRAY_BUFFER 的缓冲区。
	*/

	/*
	* 每个顶点属性从一个VBO管理的内存中获得它的数据，
	* 而具体是从哪个VBO（程序中可以有多个VBO）获取
	* 则是通过在调用glVertexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决定的。
	* 由于在调用glVertexAttribPointer之前绑定的是先前定义的VBO对象，
	* 顶点属性0现在会链接到它的顶点数据。
	*/

	/*
	* glVertexAttribPointer 仅仅是设置解析规则，默认不会启用该顶点通道
	* 通俗类比
	* VBO = 一长串连续存放数字的数组
	* glVertexAttribPointer = 一张 “说明书”，告诉 GPU：
	* 从哪一组编号 (location)、一次取几个数、是什么类型、隔多远取下一组、从头开始读还是跳过一段。
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	/*glEnableVertexAttribArray(0);
	* Enable：启用
	* Vertex：顶点
	* Attrib = Attribute：属性
	* Array：数组
	*
	* 函数作用：
	* 启用指定 location 编号的顶点属性通道。
	* glVertexAttribPointer 只是定义数据读取规则，只是写好了说明书；但通道默认处于关闭状态。
	* 调用本函数后，GPU 在绘制时才会按照你设定的规则，
	* 从 VBO 读取顶点数据送入着色器对应的 in 属性。
	*/
	/*顶点属性：
	* 通俗理解：顶点身上携带的一组数据
	* 一个顶点不只有空间坐标 (x,y,z)，还可以附带别的信息：
	* 纹理 uv、颜色、法向量等，每一类数据就叫一个顶点属性。
	* 顶点空间坐标也是一个顶点属性。
	* 举例子：
	* 属性 0：layout(location=0) in vec3 aPos;		顶点的位置坐标
	* 属性 1：layout(location=1) in vec3 aColor;		顶点的颜色
	* 属性 2：layout(location=2) in vec2 aTexCoord;	纹理坐标
	*/
	glEnableVertexAttribArray(0);
	/*
	* 当需要配置多个VAO的时候：
	* glBindVertexArray(另一个VAOID)，这一行本身就会自动把上一个 VAO 取消激活。
	* OpenGL 同一时刻，只能有 1 个 VAO 处于激活绑定状态。
	* 但是最后一个VAO配置完成以后还是建议写上glBindVertexArray(0);来解绑
	*/
	glBindVertexArray(0);  //解绑VAO
	while (!glfwWindowShouldClose(window))
	{
		processKey(window);
		glfwPollEvents();
		/*
		* glUseProgram(program) 和 glBindVertexArray(VAO) 二者之间不分先后顺序。
		* 一个是启用当前配置的着色器一个是打开当前配置的顶点数据
		* 日常写代码大家习惯性统一：先 useProgram，再 bindVAO，只是编码风格，不是语法强制。
		*/

		//先清屏，清除上一帧，然后绘图渲染，再交换
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		/*glDrawArrays(GL_TRIANGLES, 0, 3);
		* Draw：绘制
		* Arrays：数组（指 VBO 里的顶点数组）
		* 
		* 第一个参数 mode：GL_TRIANGLES
		* 图元类型，告诉 GPU 拿顶点拼成什么图形。
		* GL_TRIANGLES：每3 个顶点组成 1 个独立三角形。
		* 0,1,2  拼成第 1 个三角形
		* 如果有 6 个顶点：顶点 0-1-2 一个三角，顶点 3-4-5 另一个三角。
		* 其他常见 mode（现在不用）：
		* GL_LINES线段、GL_POINTS点
		* 第二个参数 first：0
		* 从第几个顶点开始读取。
		* 0：从顶点数组的第 0 号顶点（第一个顶点）开始。
		* 如果你写1，就跳过第 0 个，从第 1 个顶点往后读。
		* 第三个参数 count：3
		* 一共读取多少个顶点。
		* 你vertices数组正好 3 个顶点，所以填3。
		* GPU 拿 3 个顶点 + mode=GL_TRIANGLES，正好输出 1 个三角形。
		*/
		glDrawArrays(GL_TRIANGLES, 0, 3);

		
		glfwSwapBuffers(window);
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);//真正销毁释放 VAO 资源，程序结束前一定要做。
	return 0;
}