#ifndef PIC_PREVIEW_RENDER_H
#define PIC_PREVIEW_RENDER_H
#include "../libcommon/CommonTools.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "./pic_preview_texture.h"

//Shader.frag文件内容
static const char* PIC_PREVIEW_FRAG_SHADER_2 =
		"varying highp vec2 v_texcoord;\n"
		"uniform sampler2D yuvTexSampler;\n"
		"void main() {\n"
		"  gl_FragColor = texture2D(yuvTexSampler, v_texcoord);\n"
		"}\n";

//Shader.vert文件内容
static const char* PIC_PREVIEW_VERTEX_SHADER_2 =
		"attribute vec4 position;    \n"
		"attribute vec2 texcoord;   \n"
		"varying vec2 v_texcoord;     \n"
		"void main(void)               \n"
		"{                            \n"
		"   gl_Position = position;  \n"
		"   v_texcoord = texcoord;  \n"
		"}                            \n";

/*
 * 计算顶点坐标
 v_texcoord = texcoord;

取出二维纹理中该纹理坐标点上的纹理像素值
 texture2D(yuvTexSampler, v_texcoord);

 vec4 gl_Position用来设置顶点转换到屏幕坐标的位置，Vertex Shader一定要去更新这个数值

 float gl_pointSize; 在粒子效果的场景下，需要为粒子设置大小，改变该内置变量的值就是为了设置每一个粒子矩形的大小

 vec4 gl_FragColor; 用于指定当前纹理坐标所代表的像素点的最终颜色
 */
/**
 * Video OpenGL View
 */
class PicPreviewRender {
protected:
	GLint _backingLeft;
	GLint _backingTop;
	GLint _backingWidth;
	GLint _backingHeight;

	GLuint vertShader;
	GLuint fragShader;

	PicPreviewTexture* picPreviewTexture;
	GLint uniformSampler;
	GLuint program;

	int useProgram();
	int initShaders();
	GLuint compileShader(GLenum type, const char *sources);
	bool checkGlError(const char* op);

public:
	PicPreviewRender();
	virtual ~PicPreviewRender();
	virtual bool init(int width, int height, PicPreviewTexture* picPreviewTexture);
	virtual void render();
	virtual void dealloc();
	void resetRenderSize(int left, int top, int width, int height);
};

#endif //PIC_PREVIEW_RENDER_H
