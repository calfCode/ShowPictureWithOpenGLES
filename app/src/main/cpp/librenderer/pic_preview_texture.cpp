#include "./pic_preview_texture.h"

#define LOG_TAG "PicPreviewTexture"

PicPreviewTexture::PicPreviewTexture() {
}

PicPreviewTexture::~PicPreviewTexture() {
}

bool PicPreviewTexture::createTexture() {
	LOGI("enter PicPreviewTexture::createTexture");
	texture = 0;
	int ret = initTexture();
	if (ret < 0) {
		LOGI("init texture failed...");
		this->dealloc();
		return false;
	}
	return true;
}

void PicPreviewTexture::updateTexImage(byte* pixels, int frameWidth, int frameHeight) {
	if (pixels) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		if (checkGlError("glBindTexture")) {
			return;
		}
        //这样就可以将该RGBA的数组表示的像素内容上传到显卡里面texId 所代表的纹理对象中去了
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}
}

bool PicPreviewTexture::bindTexture(GLint uniformSampler) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (checkGlError("glBindTexture")) {
		return false;
	}
	glUniform1i(uniformSampler, 0);
	return true;
}

int PicPreviewTexture::initTexture() {
    // 产生纹理
	glGenTextures(1, &texture);
    // 绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture);
    /*
        一般在视频的渲染与处理的时候使用GL_LINEAR这种过滤方式， 该过滤方式称为双线性过滤，可使用双线性插值平滑像素之间的过渡， OpenGL会使用四个邻接的纹理元素，
        并在它们之间用一个线性插值算法做插值，该过滤方式是最主要的过滤方式，
    */
    // 配置的纹理对象的纹理过滤器来处理放大
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 配置的纹理对象的纹理过滤器来处理缩小
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //在纹理坐标系的s轴和t轴的纹理映射过程中用到的重复映射或者约简映射的规则,GL_CLAMP_TO_EDGE类型，因为纹理坐标可以超出（0，1）的范 围，而按照上述设置规则，所有大于1的纹理值都要设置为1，所有小于 0的值都要置为0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return 1;
}

bool PicPreviewTexture::checkGlError(const char* op) {
	GLint error;
	for (error = glGetError(); error; error = glGetError()) {
		LOGI("error::after %s() glError (0x%x)\n", op, error);
		return true;
	}
	return false;
}

void PicPreviewTexture::dealloc() {
	LOGI("enter PicPreviewTexture::dealloc");
	if (texture) {
		glDeleteTextures(1, &texture);
	}
}
