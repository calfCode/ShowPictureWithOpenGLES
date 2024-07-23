#include "./pic_preview_render.h"

#define LOG_TAG "PicPreviewRender"
enum {
	ATTRIBUTE_VERTEX, ATTRIBUTE_TEXCOORD,
};
PicPreviewRender::PicPreviewRender() {

}
PicPreviewRender::~PicPreviewRender() {

}

bool PicPreviewRender::init(int width, int height, PicPreviewTexture* picPreviewTexture) {
	this->_backingLeft = 0;
	this->_backingTop = 0;
	this->_backingWidth = width;
	this->_backingHeight = height;
	this->picPreviewTexture = picPreviewTexture;
	//初始化shaders、program、textures
	vertShader = 0;
	fragShader = 0;
	program = 0;
	int ret = initShaders();
	if(ret < 0){
		LOGI("init shader failed...");
		this->dealloc();
		return false;
	}
	ret = useProgram();
	if(ret < 0){
		LOGI("use program failed...");
		this->dealloc();
		return false;
	}
	return true;
}

void PicPreviewRender::resetRenderSize(int left, int top, int width, int height) {
	this->_backingLeft = left;
	this->_backingTop = top;
	this->_backingWidth = width;
	this->_backingHeight = height;
}

void PicPreviewRender::render(){
    // 设置窗口大小
	glViewport(_backingLeft, _backingTop, _backingWidth, _backingHeight);
    // 清屏颜色，背景色
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 使用显卡绘制程序
	glUseProgram(program);

    // 整个区域
	static const GLfloat _vertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
    // 右上角4分之一区域
//    static const GLfloat _vertices[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	glVertexAttribPointer(ATTRIBUTE_VERTEX, 2, GL_FLOAT, 0, 0, _vertices);
	glEnableVertexAttribArray(ATTRIBUTE_VERTEX);

    // 整个图片
	static const GLfloat texCoords[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
    // 部分图片
//    static const GLfloat texCoords[] = { 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.5f };
	glVertexAttribPointer(ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, 0, 0, texCoords);
	glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);
	picPreviewTexture->bindTexture(uniformSampler);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void PicPreviewRender::dealloc() {
	if (vertShader)
		glDeleteShader(vertShader);

	if (fragShader)
		glDeleteShader(fragShader);

	if(picPreviewTexture){
		picPreviewTexture->dealloc();
	}

	if (program) {
		glDeleteProgram(program);
		program = 0;
	}
}

int PicPreviewRender::useProgram() {
    // 创建程序的容器，返回容器的句柄
	program = glCreateProgram();
    // shader附着到程序
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
    // 绑定属性
	glBindAttribLocation(program, ATTRIBUTE_VERTEX, "position");
	glBindAttribLocation(program, ATTRIBUTE_TEXCOORD, "texcoord");
    // 链接程序
	glLinkProgram(program);
    // 查看链接结果
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		LOGI("Failed to link program %d", program);
		return -1;
	}
    // 使用程序
	glUseProgram(program);

	uniformSampler = glGetUniformLocation(program, "yuvTexSampler");

	return 1;
}

int PicPreviewRender::initShaders() {
	vertShader = compileShader(GL_VERTEX_SHADER, PIC_PREVIEW_VERTEX_SHADER_2);
	if (!vertShader){
		return -1;
	}
	fragShader = compileShader(GL_FRAGMENT_SHADER, PIC_PREVIEW_FRAG_SHADER_2);
	if (!fragShader){
		return -1;
	}
	return 1;
}

bool PicPreviewRender::checkGlError(const char* op) {
	GLint error;
	for (error = glGetError(); error; error = glGetError()) {
		LOGI("error::after %s() glError (0x%x)\n", op, error);
		return true;
	}
	return false;
}

GLuint PicPreviewRender::compileShader(GLenum type, const char *sources) {
	GLint status;
    // 创建shader的容器，返回容器的句柄  type为GL_VERTEX_SHADER/GL_FRAGMENT_SHADER
	GLuint shader = glCreateShader(type);
	if (shader == 0 || shader == GL_INVALID_ENUM) {
		LOGI("Failed to create shader %d", type);
		return 0;
	}
    // 为shader添加源码
	glShaderSource(shader, 1, &sources, NULL);
    // 编译shader
	glCompileShader(shader);
    // 查看编译结果
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		glDeleteShader(shader);
		LOGI("Failed to compile shader : %s\n", sources);
		return 0;
	}
	return shader;
}
