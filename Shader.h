//#pragma once  //非标准编译器扩展指令：不是C++标准，
// 但是常见编译环境都能用，只是一些极少数古老冷门编译器不支持

//#ifndef  #define  #endif 头文件宏守卫：C++语言标准特性
//#ifndef//if-not-defined,如果这个宏尚未定义
//#define:定义一个预处理宏
// 
// 头文件内容
//endif:结束条件编译块
#pragma once
#include<string>
class Shader
{
public:
	//程序ID
	unsigned int ID;
	//构造函数
	Shader(const char* vertexPath, const char* fragmentPath, const char* defines = nullptr);
	//使用/激活程序
	void use() const;
	//uniform工具函数
	void setBool(const std::string &name, bool value)const;
	void setInt(const std::string& name, int value)const;
	void setFloat(const std::string& name, float value)const;
};

//类外面，仍然写在.h  必须手动加 inline
//就是如果想在这里写函数的实现就要写成 inline Shader::Shader(const char* vertexPath,const …………