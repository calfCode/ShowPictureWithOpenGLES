#include <jni.h>
#include <string>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "CommonTools.h"
#include "librenderer/pic_preview_controller.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>

#define LOG_TAG "BRUCE"

//EGLConfig m_eglConf;
//extern "C" JNIEXPORT jstring JNICALL
//Java_com_smartdevice_opengles_MainActivity_stringFromJNI(
//        JNIEnv* env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}
//extern "C"
//JNIEXPORT void JNICALL
//Java_com_smartdevice_opengles_MainActivity_openglesDemo(JNIEnv *env, jobject thiz,jobject surfaceObj) {
//    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env,surfaceObj);
//    EGLDisplay display;
//    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
//        LOGE("eglGetDisplay() returned error %d", eglGetError());
//        return ;
//    }
//    if (!eglInitialize(display, 0, 0)) {
//        LOGE("eglInitialize() returned error %d", eglGetError());
//        return ;
//    }
//
//    const EGLint attribs[] = {EGL_BUFFER_SIZE, 32,
//                              EGL_ALPHA_SIZE, 8,
//                              EGL_BLUE_SIZE, 8,
//                              EGL_GREEN_SIZE, 8,
//                              EGL_RED_SIZE, 8,
//                              EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
//                              EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
//                              EGL_NONE };
//    EGLint numConfigs;
//    if (!eglChooseConfig(display, attribs, &m_eglConf, 1, &numConfigs)) {
//        LOGE("eglChooseConfig() returned error %d", eglGetError());
//    }
//    EGLContext context;
//    EGLint attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
//    if (!(context = eglCreateContext(display, m_eglConf, NULL,
//                                     attributes))) {
//        LOGE("eglCreateContext() returned error %d", eglGetError());
//        return ;
//    }
//    EGLSurface surface = NULL;
//    EGLint format;
//    if (!eglGetConfigAttrib(display, m_eglConf, EGL_NATIVE_VISUAL_ID,
//                            &format)) {
//        LOGE("eglGetConfigAttrib() returned error %d", eglGetError());
//        return ;
//    }
//    ANativeWindow_setBuffersGeometry(nativeWindow, 0, 0, format);
//    if (!(surface = eglCreateWindowSurface(display, m_eglConf, nativeWindow, 0))) {
//        LOGE("eglCreateWindowSurface() returned error %d", eglGetError());
//    }
//    // 6.绑定上下文
//    if (!eglMakeCurrent(display,surface,surface,context)){
//        LOGE("eglMakeCurrent() returned error %d", eglGetError());
//        return;
//    }
//    GLuint texId;
//    glGenTextures(1, &texId);
//    glBindTexture(GL_TEXTURE_2D,texId);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//    glBindTexture(GL_TEXTURE_2D,0);
//    return;
//}


static ANativeWindow *window = 0;
static PicPreviewController *controller = 0;
extern "C"
JNIEXPORT void JNICALL Java_com_smartdevice_opengles_PngPreviewController_init
        (JNIEnv * env, jobject obj, jstring pngFilePathParam) {
    controller = new PicPreviewController();
    char* pngFilePath = (char*) env->GetStringUTFChars(pngFilePathParam, NULL);
//    LOGI("init pngFilePath=%s",pngFilePath);
    controller->start(pngFilePath);
    env->ReleaseStringUTFChars(pngFilePathParam, pngFilePath);
//    return;
}
extern "C"
JNIEXPORT void JNICALL Java_com_smartdevice_opengles_PngPreviewController_setSurface
        (JNIEnv * env, jobject obj, jobject surface) {
    if (surface != 0 && NULL != controller) {
        window = ANativeWindow_fromSurface(env, surface);
        LOGI("Got window %p", window);
        controller->setWindow(window);
    } else if (window != 0) {
        LOGI("Releasing window");
        ANativeWindow_release(window);
        window = 0;
    }
}
extern "C"
JNIEXPORT void JNICALL Java_com_smartdevice_opengles_PngPreviewController_resetSize
        (JNIEnv * env, jobject obj, jint width, jint height) {
    if (NULL != controller) {
        controller->resetSize(width, height);
    }
}
extern "C"
JNIEXPORT void JNICALL Java_com_smartdevice_opengles_PngPreviewController_stop
        (JNIEnv * env, jobject obj) {
    if (NULL != controller) {
        controller->stop();
        delete controller;
        controller = NULL;
    }
}