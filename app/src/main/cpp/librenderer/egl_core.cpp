#include "egl_core.h"

#define LOG_TAG "EGLCore"

EGLCore::EGLCore() {
	pfneglPresentationTimeANDROID = 0;
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
}

EGLCore::~EGLCore() {
}

void EGLCore::release() {
	eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	LOGI("after eglMakeCurrent...");
	eglDestroyContext(display, context);
	LOGI("after eglDestroyContext...");
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
}

void EGLCore::releaseSurface(EGLSurface eglSurface) {
	eglDestroySurface(display, eglSurface);
	eglSurface = EGL_NO_SURFACE;
}

EGLContext EGLCore::getContext(){
	LOGI("return EGLCore getContext...");
	return context;
}

EGLDisplay EGLCore::getDisplay(){
	return display;
}

EGLConfig EGLCore::getConfig(){
	return config;
}

EGLSurface EGLCore::createWindowSurface(ANativeWindow* _window) {
	EGLSurface surface = NULL;
	EGLint format;
	if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
		LOGE("eglGetConfigAttrib() returned error %d", eglGetError());
		release();
		return surface;
	}
	ANativeWindow_setBuffersGeometry(_window, 0, 0, format);
    //那么如何将EGL和设备的屏幕连接起来呢？答案是使用EGLSurface，Surface实际上是一个FrameBuffer，通过 EGL库提供的eglCreateWindowSurface可以创建一个可实际显示的Surface
	if (!(surface = eglCreateWindowSurface(display, config, _window, 0))) {
		LOGE("eglCreateWindowSurface() returned error %d", eglGetError());
	}
	return surface;
}

EGLSurface EGLCore::createOffscreenSurface(int width, int height) {
	EGLSurface surface;
	EGLint PbufferAttributes[] = { EGL_WIDTH, width, EGL_HEIGHT, height, EGL_NONE, EGL_NONE };
	if (!(surface = eglCreatePbufferSurface(display, config, PbufferAttributes))) {
		LOGE("eglCreatePbufferSurface() returned error %d", eglGetError());
	}
	return surface;
}

int EGLCore::setPresentationTime(EGLSurface surface, khronos_stime_nanoseconds_t nsecs) {
	pfneglPresentationTimeANDROID(display, surface, nsecs);
}

int EGLCore::querySurface(EGLSurface surface, int what) {
	int value = -1;
	eglQuerySurface(display, surface, what, &value);
	return value;
}

bool EGLCore::swapBuffers(EGLSurface eglSurface) {
	return eglSwapBuffers(display, eglSurface);
}

bool EGLCore::makeCurrent(EGLSurface eglSurface) {
    //每个线程都需要绑定一个上下文，这样才可以执行OpenGL的指令，所以首先需要调用eglMakeCurrent，来为该 线程绑定Surface与Context
	return eglMakeCurrent(display, eglSurface, eglSurface, context);
}

void EGLCore::doneCurrent() {
	eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

bool EGLCore::init() {
	return this->init(NULL);
}

bool EGLCore::init(EGLContext sharedContext) {
	EGLint numConfigs;
	EGLint width;
	EGLint height;


    // 首先EGL需要知道绘制内容的目标在哪里，EGLDisplay是一个封装 系统物理屏幕的数据类型
	if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
		LOGE("eglGetDisplay() returned error %d", eglGetError());
		return false;
	}
    //初始化这个显示设备.后面两个参数则代表Major和Minor的版本， 比如EGL的版本号是1.0，那么Major将返回1，Minor则返回0。如果不关 心版本号，则可都传入0或者NULL
	if (!eglInitialize(display, 0, 0)) {
		LOGE("eglInitialize() returned error %d", eglGetError());
		return false;
	}
    //准备配置选项了，一旦EGL有了Display之后，它就可 以将OpenGL ES的输出和设备的屏幕桥接起来，但是需要指定一些配置 项，类似于色彩格式、像素格式、RGBA的表示以及SurfaceType等
    const EGLint attribs[] = { EGL_BUFFER_SIZE, 32, EGL_ALPHA_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                               EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE };
	if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
		LOGE("eglChooseConfig() returned error %d", eglGetError());
		release();
		return false;
	}
    // 创建OpenGL的上下文环境
	EGLint eglContextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	if (!(context = eglCreateContext(display, config, NULL == sharedContext ? EGL_NO_CONTEXT : sharedContext, eglContextAttributes))) {
		LOGE("eglCreateContext() returned error %d", eglGetError());
		release();
		return false;
	}

	pfneglPresentationTimeANDROID = (PFNEGLPRESENTATIONTIMEANDROIDPROC)eglGetProcAddress("eglPresentationTimeANDROID");
	if (!pfneglPresentationTimeANDROID) {
		LOGE("eglPresentationTimeANDROID is not available!");
	}

	return true;
}
