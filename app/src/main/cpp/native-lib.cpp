#include <jni.h>
#include <string>

#include "AAudioEcho.h"

AAudioEcho *echo = NULL;

extern "C" JNIEXPORT void JNICALL
Java_com_zu_aaudiotest_MainActivity_nInit(JNIEnv *env, jobject instance)
{
    if(!echo)
    {
        echo = new AAudioEcho();
        echo->init();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_zu_aaudiotest_MainActivity_nDestroy(JNIEnv *env, jobject instance)
{
    if(echo)
    {
        echo->destroy();
        delete(echo);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_zu_aaudiotest_MainActivity_nStart(JNIEnv *env, jobject instance)
{
    if(!echo)
    {
        return;
    }
    echo->start();
}

extern "C" JNIEXPORT void JNICALL
Java_com_zu_aaudiotest_MainActivity_nStop(JNIEnv *env, jobject instance)
{
    if(!echo)
    {
        return;
    }
    echo->stop();
}



