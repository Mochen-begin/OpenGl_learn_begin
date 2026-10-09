//类名是 'Shader'，文件和类名保持一模一样，C++ 最普遍约定。
//#pragma once 是头文件保护命令，.cpp文件不应该加
#include<glad/glad.h>
#include"Shader.h"
#include<fstream>
#include<sstream>
#include<iostream>
#include<string>
Shader::Shader(const char* vertexPath, const char* fragmentPath,const char* defines)
{
	//1,从文件路径中获取顶点/片段着色器
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//保证ifstream对象可以抛出异常
	/*stream.exceptions(掩码)
	* 默认情况下：C++ IO 流不会抛异常！
	* 文件打开失败、读取出错，流只会把内部状态标记位（`failbit`/`badbit`）置 1，
	* 代码继续往下跑，不会中断，需要你手动写`if(!file)`判断。
	* stream.exceptions(掩码)`这个函数的作用：
	* 设置掩码：**当流出现掩码指定的错误状态时，
	* 自动抛出 `std::ios_base::failure`异常**，不再需要手动`if(!vShaderFile)`判断。
	* `|`是按位或，表示同时监听两种错误标志：
	* `failbit` **或者** `badbit`只要任意一个触发，就抛异常
	* 
	* 
	* | `goodbit` | 0，一切正常 | 无错误 |
	* | --- | --- | --- |
	* | `failbit` | 逻辑错误，操作失败 | 文件打不开；读取格式错误；文件不存在；open 失败 |
	* | `badbit` | 严重底层损坏 | 磁盘 IO 故障、缓冲区损坏，流对象彻底坏掉 |
	* | `eofbit` | 读到文件末尾 | 正常结束，**一般不把它放进异常掩码**，读到文件末尾不是错误GCC |
	* 标准写法顺序：**先调用`.exceptions()`，再调用`.open()`打开文件**。
	* 设置异常掩码之后，**必须用 try‑catch 包裹 IO 代码，否则抛出异常没有捕获，程序直接崩溃退出**。
	*/
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//读取文件的缓冲内容到数据流中
		/*rdbuf()---read buffer 读取缓冲
		* 每一个 C++ IO 流对象（`ifstream/cout/stringstream`）内部都有一个**缓冲区对象`streambuf`**，这才是真正存放字节数据的底层。
		*  `vShaderFile.rdbuf()` 返回文件流底层缓冲区的**指针 `streambuf*`**。
		* `operator<<` 运算符有一个特殊重载：
		*  当`<<`右边接收一个`streambuf*`的时候，会**直接把源缓冲区全部字节原样复制过来**，不需要一行一行手动读。
		*/
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		//转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		if (defines != nullptr)
		{
			//size_t是无符号整数，是find等函数的标准返回类型，由于不同平台返回类型不一样（有的是unsigned int,有的是unsigned long long)但是size_t是平台typedef起的别名所以会符合平台具体类型
			size_t index = fragmentCode.find('\n');
			fragmentCode.insert(index + 1, std::string(defines) + '\n');
			
		}
		
		/*
		* 上面是通过stringstream来做到从ifstream到string，在大型工程上会选用C++17的更高性能的方式
		* std::string vertexCode(std::istreambuf_iterator<char>(vShaderFile), std::istreambuf_iterator<char>{});
		* vShaderFile.close();
		*/
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	/*c_str()成员函数
	* - `c_str`：c‑string 的缩写，**C 风格字符串**
	* - 返回值：`const char*`，老式 C 语言的以 `\0` 结尾的字符指针。是一个常量，不可修改
	*/
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	//2，编译着色器
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILEC\n" << infoLog << std::endl;
	}

	//片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILEC\n" << infoLog << std::endl;
	}
	//着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//打印连接错误（如果有的话）
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//删除着色器
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
//使用/激活程序
void Shader::use() const
{
	glUseProgram(ID);
}
//uniform工具函数
void Shader::setBool(const std::string& name, bool value)const
{
	// glUniform1i接受的类型本身就是int类型，并且GLSL 的 `bool` 类型 uniform，CPU 侧就是用整数 0/1 去赋值。
	//所以这里要强制类型转换
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value)const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}