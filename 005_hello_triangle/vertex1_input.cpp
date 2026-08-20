#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
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
		std::cout << "Failed to initialize GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed load glad" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);
	/*接下来这些是初始化，不能写在循环内部*/
	/*
	* 在OpenGL中，任何事物都在3D空间中，而屏幕和窗口却是2D像素数组，
	* 这导致OpenGL的大部分工作都是关于把3D坐标转变为适应你屏幕的2D像素。
	* 3D坐标转为2D坐标的处理过程是由OpenGL的图形渲染管线
	* （Graphics Pipeline，大多译为管线，实际上指的是一堆原始图形数据途经一个输送管道，
	* 期间经过各种变化处理最终出现在屏幕的过程）管理的
	*
	* 图形渲染管线可以被划分为两个主要部分：
	* 第一部分把你的3D坐标转换为2D坐标，
	* 第二部分是把2D坐标转变为实际的有颜色的像素。
	*/

	/*渲染的过程：
	* 图形渲染管线的第一个部分是顶点着色器(Vertex Shader)，它把一个单独的顶点作为输入。
	* 顶点着色器主要的目的是把3D坐标转为另一种3D坐标（后面会解释），
	* 同时顶点着色器允许我们对顶点属性进行一些基本处理。
	* 顶点着色器阶段的输出可以选择性地传递给几何着色器(Geometry Shader)。
	* 几何着色器将一组顶点作为输入，这些顶点形成图元，
	* 并且能够通过发出新的顶点来形成新的(或其他)图元来生成其他形状。
	* 在这个例子中，它从给定的形状中生成第二个三角形。
	* 图元装配(Primitive Assembly)阶段将顶点着色器（或几何着色器）
	* 输出的所有顶点作为输入（如果是GL_POINTS，那么就是一个顶点），
	* 并将所有的点装配成指定图元的形状；本节例子中是两个三角形。
	* 图元装配阶段的输出会被传入光栅化阶段(Rasterization Stage)，
	* 这里它会把图元映射为最终屏幕上相应的像素，
	* 生成供片段着色器(Fragment Shader)使用的片段(Fragment)。
	* 在片段着色器运行之前会执行裁切(Clipping)。
	* 裁切会丢弃超出你的视图以外的所有像素，用来提升执行效率。
	* OpenGL中的一个片段是OpenGL渲染一个像素所需的所有数据。
	* 片段着色器的主要目的是计算一个像素的最终颜色，
	* 这也是所有OpenGL高级效果产生的地方。通常，
	* 片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），
	* 这些数据可以被用来计算最终像素的颜色。
	* 在所有对应颜色值确定以后，最终的对象将会被传到最后一个阶段，
	* 我们叫做Alpha测试和混合(Blending)阶段。
	* 这个阶段检测片段的对应的深度（和模板(Stencil)）值（后面会讲），
	* 用它们来判断这个像素是其它物体的前面还是后面，决定是否应该丢弃。
	* 这个阶段也会检查alpha值（alpha值定义了一个物体的透明度）并对物体进行混合(Blend)。
	* 所以，即使在片段着色器中计算出来了一个像素输出的颜色，
	* 在渲染多个三角形的时候最后的像素颜色也可能完全不同。
	*/

	/*
	* 然而，对于大多数场合，我们只需要配置顶点和片段着色器就行了。
	* 几何着色器是可选的，通常使用它默认的着色器就行了。
	* 在现代OpenGL中，
	* 我们必须定义至少一个顶点着色器和一个片段着色器
	* （因为GPU中没有默认的顶点/片段着色器）。
	*/

	/*
	* 标准化设备坐标：x,y,z三个值都在-1.0，1.0之间，在这个范围之内时才会处理它，
	* 所有在这个范围之外的坐标则不会显示
	*
	* OpenGL 可见范围固定三轴 [-1.0, 1.0]，只是数值区间限制，不限制小数点位数：
	* 坐标 0.999f 在范围内，能显示；
	* 坐标 1.001f 超出区间，直接裁剪看不见；
	* 和小数点有几位没有关系。
	*/

		/*定义一个三角形顶点数组*/
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	/*
	* 单数：vertex  单个顶点
	* 复数：vertices （图形学标准复数，代码里数组名通用这个）
	* 中文：顶点集合、全部顶点
	*/

	/*深度：
	* 通常深度可以理解为z坐标，它代表一个像素在空间中和你的距离，
	* 如果离你远就可能被别的像素遮挡，你就看不到它了，它会被丢弃，以节省资源
	*/

	/*
	* VBO = Vertex Buffer Object 顶点缓冲对象
	* 作用：在 GPU 显存开辟一块内存，
	* 存放你写的 vertices 顶点数组，避免 CPU 反复传数据拖慢性能
	*/

	/*
	* 1, OpenGL 对象的通用规则
	* OpenGL 所有资源（缓冲、纹理、VAO、帧缓冲）都遵循一套逻辑：
	* 1,每个资源有唯一无符号整数 ID（相当于资源的身份证号）；
	* 2,调用 glGenXXX 系列函数，让 OpenGL 分配 ID；
	* 3,用 ID 绑定、操作、销毁对应资源。
	*/

	/*
	* glGenBuffers(1, &VBO);
	* 函数原型：void glGenBuffers(GLsizei n, GLuint *buffers);
	* 第一个参数 1：你要生成几个缓冲 ID，这里只创建 1 个 VBO，写 1；
	* 想一次性创建 3 个就写 3，传数组地址；
	* 第二个参数 &VBO：变量地址，OpenGL 会把生成好的唯一 ID 写入这个变量；
	* 执行完这行后：VBO 变量里就有合法缓冲 ID，代表一块 GPU 顶点内存。
	*/
	/*缓冲：
	* 缓冲 = 一块连续显存
	* ① 一块显存只能属于一个缓冲
	* 不能一块显存同时分给两个 ID，一个缓冲也不能由多段零散显存拼接，缓冲内存必须连续。
	* 如果你执行两次 glBufferData 给同一个 ID：会释放旧显存，再申请一块全新显存，缓冲只会持有当前这一块。
	* ② 显存里不只有缓冲
	* 显卡 16G 显存是公共大池子，里面存放多种资源，缓冲只是其中一类：
	* 缓冲（VBO/EBO/UBO）：顶点、索引、通用计算数据；
	* 纹理贴图：图片、材质贴图；
	* 帧缓冲、渲染缓冲区：屏幕画面、离屏渲染缓存；
	* 驱动内部预留、GPU 程序、常量内存等。
	* 所以：显存 ≠ 缓冲，缓冲只是显存里的一块分区
	*/
	/*
	* 个缓冲 ID 绑定 1 块独立、连续的显存区块，这块区块容量自定义，没有固定大小；
	* 一个 ID 只能对应一块显存，不能一块 ID 对应多段分散显存，缓冲内存必须连续；
	* 多个不同 ID = 多块独立显存，互不占用对方空间；
	* ID ≠ 硬件地址，是驱动给你的抽象编号，内部映射真实显存地址；
	*/
	/*
	* unsigned int：C++ 基础无符号整型，OpenGL 所有对象 ID 统一用这个类型存储；
	* VBO：自定义变量名，只是代码里用来存缓冲 ID 的容器，没有语法绑定；
	* glGenBuffers(数量, 变量地址)：OpenGL 函数，只负责生成 ID 数字，
	* 写入你传进去的变量，不关心变量叫什么。
	*/
	/*
	* 缓冲底层硬件内存块本身没有区别，区分用途完全靠绑定的 Target 目标 + 你的使用逻辑。
	* 一块 GPU 显存缓冲，天生只是一块空白连续内存，
	* 没有 “顶点缓冲 / 索引缓冲 / 常量缓冲” 的固有标签，
	* 只是你绑定到不同 Target、用对应接口读写，才赋予它对应的功能。
	*
	* 底层层面：所有缓冲完全同质
	* 内存结构统一：
	* glGenBuffers 创建出来的缓冲，只是 GPU 上一块普通连续显存，没有内置类型标记，
	* 存储的都是二进制字节流，放什么数据完全由你决定。
	* ID 无专属分类：
	* 所有缓冲 ID 都是 unsigned int，
	* glGenBuffers 生成的数字没有区分 VBO/EBO/UBO，
	* 驱动内部不区分缓冲本身，只区分绑定通道。
	* 大小、读写权限规则通用：
	* glBufferData、glBufferSubData、glDeleteBuffers 对任意缓冲调用逻辑完全一致，
	* 分配内存、更新数据、销毁资源的底层操作没有差异。
	*/

	/*
	* VBO 是CPU 内存（电脑内存条）里的普通 C++ 无符号整型变量。
	* 它只存一个数字（OpenGL 缓冲句柄 ID，比如 1、2、5），占用 4 字节内存，
	* 和显卡显存完全无关。
	* &VBO 是这个 CPU 变量的内存地址，传给glGenBuffers，
	* 函数把生成的缓冲 ID 写入这个 CPU 变量。
	* 2. 真正的缓冲数据在哪？（显卡显存）
	* glGenBuffers 做两件事：
	* 向显卡驱动申请一块 GPU 显存（真正存放顶点数据的内存）；
	* 驱动给这块显存分配一个唯一数字 ID，把 ID 写到你 CPU 内存的VBO变量里。
	* 层级拆分
	* CPU 内存条：变量VBO，只存 ID 数字，极小；
	* 显卡显存：真正的缓冲内存块，存顶点坐标、颜色等模型数据；
	* 映射关系：驱动内部维护一张表 ID → 显存物理地址，
	* 你程序看不到显存真实地址，只能靠 ID 操作显存。
	*/

	/*
	* 二、核心本质区别（千万不能等同）
	* 1. 存储的内容完全不同
	* 指针：存硬件真实内存地址（CPU 虚拟地址 / 物理地址），
	* 操作系统直接靠这个地址访问内存；
	* VBO 的 ID：只是驱动自定义的整数句柄，不是显存地址。
	* 驱动内部有一张映射表：ID → 显存真实地址，程序永远拿不到显存原始地址。
	* 2. 寻址权限差异巨大
	* 指针：CPU 可以直接解引用 *p 读写内存，绕过任何中间层；
	* ID：程序不能直接访问显存，必须调用 OpenGL 函数（glBindBuffer/glBufferData），
	* 由驱动中转读写显存，没有任何 “解引用” 操作。
	* 3. 内存归属不同
	* 普通指针指向的内存：CPU 内存（内存条）；
	* ID 对应的内存：GPU 显存，CPU 不能直接读写，必须走 CPU→GPU 数据拷贝。
	* 4. 生命周期管理不同
	* 指针：单纯地址值，指针销毁不会释放内存，内存靠new/delete管理；
	* 缓冲 ID：glGenBuffers 自动分配显存，必须用glDeleteBuffers(id)释放显存；
	* 仅仅VBO变量销毁，显存不会自动回收，造成显存泄漏。
	* 5. 指针可以任意运算，ID 不行
	* 指针能加减偏移 p+1 跳转内存；
	* 缓冲 ID 只是单纯编号，不能做任何算术运算，VBO+1 不代表下一块缓冲，毫无意义。
	*/

	/*
	* glGenBuffers 第一次返回 1，第二次返回 2，依次递增；
	* 不会出现 ID=100，但中间 2~99 都不存在的稀疏情况
	* （就算删除缓冲，ID 也不会复用，只会持续增大）。
	* 缓冲 ID 不会重复复用：就算调用 glDeleteBuffers 销毁 ID=1 的缓冲，
	* 后续生成的新 ID 只会继续往后走（2、3、4…），不会重新分配 1；
	* 除了 Buffer以外纹理、VAO、着色器等所有 OpenGL 对象 ID，
	* 全部用这套数组下标映射逻辑；
	* 只有极少数拓展图形 API 会用哈希表管理资源，标准 OpenGL 驱动一律用数组索引。
	*/
	/*
	* 先分清两个独立步骤（很多人混淆）
	* glGenBuffers
	* 只做两件事：
	* 给你生成一个唯一递增数字 ID；
	* 在驱动内部记录：这个 ID 对应一块待分配的显存资源对象。
	* 它根本不分配显存内存，不碰那 16G 空间，没有任何 “找显存块” 操作。
	* 真正分配 16G 显存里内存的是 glBufferData /glBufferSubData
	* 只有调用这行，驱动才会去 16G 物理显存里划分一块连续内存给你的缓冲。
	* 二、16G 显存的分配算法：绝对不随机，是内存管理器统一调度
	* 显卡有一套 GPU 显存内存分配器，和操作系统管理 CPU 内存逻辑类似，核心规则：
	* 目标：分配一段连续显存空间
	* 顶点、索引缓冲都要求内存连续，不能碎块拼接。
	* 分配策略（常用两种）
	* 首次适配：从显存低地址往高地址遍历，找到第一块足够大的空闲连续区块；
	* 最佳适配：遍历所有空闲块，选大小刚好能放下、剩余碎片最小的区块。
	* 全程是确定性遍历算法，没有随机逻辑，不会随便乱挑一块。
	* 碎片处理
	* 多次创建 / 删除缓冲后，显存会出现细碎空闲块；空间不足时驱动会做显存整理（压缩、合并空闲块），或把部分缓冲挪到共享内存。
	* 上限约束
	* 所有缓冲、纹理、帧缓存占用总和不能超过 16G 物理显存。
	*/
	/*函数逻辑：
	* glGenBuffers(&VBO)
	* ID=1 生成，驱动记录表新增空条目，无显存、无地址映射；
	* glBindBuffer(GL_ARRAY_BUFFER, VBO)
	* 把 ID=1 的条目设为当前操作通道，依旧没有显存分配；
	* glBufferData(..., vertices, ...)
	* 在 16G 显存分配固定地址的内存块，将该地址写入 ID=1 的条目，映射关系正式建立且永久固定；
	* 后续绘图、glBufferSubData 修改数据：显存地址全程不变；
	* glDeleteBuffers(VBO)
	* 释放显存，清空 ID=1 条目里的显存地址，映射失效，ID 永久作废不再复用。
	*
	* gen 是 generate 的缩写
	* generate 动词本意：生成、创造、产出
	*/

	/* 生成一个带有缓冲ID的VBO对象*/
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	/*
	* OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。
	* OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。
	*/
	/*GL_ARRAY_BUFFER
	* 是一个缓冲类型，也叫做槽位，就是绑定目标Target。
	* OpenGL 驱动内部，给每一类用途单独准备了一个全局盒子，
	* 这个盒子就叫绑定目标（Target），俗称槽位。
	* 每个槽位里只存一个数字：缓冲 ID。
	* 所有槽位互相独立、互不干扰，每个盒子同一时间只能装一个 ID 数字。
	* 如果重复调用，下一个id会覆盖掉上一个id，
	* 槽位的核心就是中转状态机。
	* 槽位是驱动内置、永久存在的，程序启动就自带，不用你手动创建；
	* 每个 Target 对应独立槽位，互不干扰。
	* OpenGL很多函数没办法直接传入id而是寻找本身就存在的槽位，然后通过槽位储存的id
	* 去寻找对应需要写入数据的显存（就是缓冲）；
	*
	* 比如：
	* glBufferData 第一个参数指定槽位
	* 函数流程：读取该槽位存储的缓冲 ID
	* → 通过 ID 查表找到对应的显存区块 → 完成显存分配 / 数据写入。
	*
	* 覆盖规则：单个槽位同一时刻只能存 1 个 ID，重复绑定同槽位会替换原有 ID；
	* 不同槽位可以同时保存各自 ID，互不影响。
	* 无绑定的情况：如果槽位为空（没执行过 glBindBuffer），
	* 此时调用 glBufferData 会报错，没有 ID 就找不到显存块，无法操作。
	*/

	/*槽位：
	*  显卡硬件本身没有 “槽位” 这个东西
	* GPU 硬件只认显存地址，硬件层面逻辑非常简单：
	* 硬件：一堆独立显存块 + 顶点处理器、光栅器等运算单元
	* GPU 读取数据时，直接给一块显存的物理地址，就能读写，
	* 不存在区分「顶点槽 / 索引槽」的硬件盒子
	* 显卡硬件完全不关心这块显存是存顶点还是存索引，硬件无分类、无槽位。
	* 2. 槽位 Target 是 OpenGL API（驱动软件层）设计出来的状态机规则
	* OpenGL 是一套软件图形标准，为了简化 API 设计，人为加了这套绑定槽位机制：
	* 当年 OpenGL 初代设计选择了全局状态机架构，
	* 放弃 “直接传对象 ID 操作资源” 的函数风格；
	* 为了区分不同用途的缓冲，划分出 GL_ARRAY_BUFFER、
	* GL_ELEMENT_ARRAY_BUFFER 等独立全局状态变量（就是咱们说的槽位）；
	* 所有操作缓冲的函数，不再携带缓冲 ID 参数，
	* 而是读取对应全局状态（槽位）里存的 ID，间接操作显存。
	* 3. 横向对比验证：别的图形 API 没有这套槽位机制
	* Vulkan / DirectX12：完全没有槽位，所有操作都直接传入缓冲 ID / 显存地址，
	* 没有绑定、没有全局状态，不需要 bind 绑定；
	* DirectX 11：有少量绑定槽，但划分逻辑和 OpenGL 完全不同；
	* 这直接证明：槽位是 OpenGL 独有的软件设计，不是显卡硬件固有特性。
	*/

	/*glBindBuffer(GL_ARRAY_BUFFER,VBO)
	* bind:捆绑，绑定，关联
	* glBindBuffer:绑定缓冲
	*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/*glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW)
	* 作用：把数据写入到对应区域
	* glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。
	* 它的第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。
	*
	* 第二个参数指定传输数据的大小(以字节为单位)；
	* 用一个简单的sizeof计算出顶点数据大小就行。
	*
	* 第三个参数是我们希望发送的实际数据。
	*
	* 第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
	* GL_STATIC_DRAW ：数据不会或几乎不会改变。
	* GL_DYNAMIC_DRAW：数据会被改变很多。
	* GL_STREAM_DRAW ：数据每次绘制时都会改变。
	* 三角形的位置数据不会改变，每次渲染调用时都保持原样，
	* 所以它的使用类型最好是GL_STATIC_DRAW。如果，比如说一个缓冲中的数据将频繁被改变，
	* 那么使用的类型就是GL_DYNAMIC_DRAW或GL_STREAM_DRAW，
	* 这样就能确保显卡把数据放在能够高速写入的内存部分。
	*
	* STATIC static
	* 形容词：静态的、固定不变的
	* DYNAMIC dynamic
	* 形容词：动态的、频繁变动的
	* STREAM stream
	* 名词 / 形容词：流式、瞬时数据流、每次刷新的临时数据
	* DRAW draw
	* 动词：绘制（代表用途：GPU 拿这份数据画图）
	*
	* 完整宏标准翻译:
	* GL_STATIC_DRAW
	* 直译：静态绘制
	* 释义：顶点数据上传显存后几乎永不修改，GPU 反复读取绘图
	* GL_DYNAMIC_DRAW
	* 直译：动态绘制
	* 释义：数据会频繁修改、更新，但不会每帧都换
	* GL_STREAM_DRAW
	* 直译：流式绘制
	* 释义：每一帧绘制前都会重新上传、更新一遍全新数据
	* 补充底层原理（显卡会根据这个参数优化显存存放位置）
	* 显卡驱动会根据这个提示，把缓冲放到不同速度的显存分区：
	* STATIC：放 GPU 高速只读显存，读取极快，写入很慢（适合模型、固定三角形）
	* DYNAMIC：放可读写显存，读写速度均衡（会时不时修改顶点动画）
	* STREAM：放 CPU-GPU 互通的临时显存，专门给每帧刷新的数据用（粒子、实时动态线条）
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{

		processInput(window);
		

		
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}