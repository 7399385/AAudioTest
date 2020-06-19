//
// Created by 祖国瑞 on 2020-06-06.
//

#include "AAudioEcho.h"

#define MODULE_NAME  "AAudioEcho"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, MODULE_NAME, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, MODULE_NAME, __VA_ARGS__)

FILE *pcmFile = NULL;

aaudio_data_callback_result_t AAudioEcho::audio_callback(AAudioStream *stream, void *userData, void *audioData, int32_t numFrames)
{
    AAudioEcho *context = (AAudioEcho *)userData;
    if(context->outputStream == NULL)
    {
        return AAUDIO_CALLBACK_RESULT_STOP;
    }
    AAudioStream_write(context->outputStream, audioData, numFrames, 3000);
//    fwrite(audioData, sizeof(int16_t), numFrames, pcmFile);
    return AAUDIO_CALLBACK_RESULT_CONTINUE;


}

AAudioEcho::AAudioEcho() {
    buffer = (int16_t *)calloc(SAMPLE_COUNT, sizeof(int16_t));
//    pcmFile = fopen("/sdcard/test.pcm", "wb");
}

AAudioEcho::~AAudioEcho() {
    if(buffer)
    {
        free(buffer);
    }
//    if(pcmFile)
//    {
//        fflush(pcmFile);
//        fclose(pcmFile);
//    }
}

void AAudioEcho::init() {
    aaudio_result_t result;
    AAudioStreamBuilder *builder;
    result = AAudio_createStreamBuilder(&builder);
    if(result != AAUDIO_OK)
    {
        LOGE("create stream builder error");
        return;
    }

    AAudioStreamBuilder_setBufferCapacityInFrames(builder, SAMPLE_COUNT);
    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
    AAudioStreamBuilder_setChannelCount(builder, 1);
    AAudioStreamBuilder_setSampleRate(builder, SAMPLE_RATE);

    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_INPUT);

    AAudioStreamBuilder_setDataCallback(builder, audio_callback, this);
    result = AAudioStreamBuilder_openStream(builder, &inputStream);
    if(result != AAUDIO_OK)
    {
        LOGE("open record stream failed");
        return;
    }

    AAudioStreamBuilder_setChannelCount(builder, 1);
    AAudioStreamBuilder_setDataCallback(builder, NULL, NULL);
    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
    result = AAudioStreamBuilder_openStream(builder, &outputStream);
    if(result != AAUDIO_OK)
    {
        LOGE("open play stream failed");
        return;
    }




    AAudioStreamBuilder_delete(builder);

}

void AAudioEcho::destroy() {
    if(inputStream)
    {
        AAudioStream_close(inputStream);
    }

    if(outputStream)
    {
        AAudioStream_close(outputStream);
    }
}

void AAudioEcho::start() {
    if(!inputStream)
    {
        LOGE("input stream not created");
        return;
    }
    if(!outputStream)
    {
        LOGE("output stream not created");
        return;
    }
    if(AAudioStream_getState(inputStream) == AAUDIO_STREAM_STATE_STARTED)
    {
        LOGE("stream already started");
    }
    aaudio_result_t result;
    aaudio_stream_state_t inputState = AAUDIO_STREAM_STATE_STARTING;
    aaudio_stream_state_t nextState = AAUDIO_STREAM_STATE_UNINITIALIZED;
    result = AAudioStream_requestStart(outputStream);
    if(result != AAUDIO_OK)
    {
        LOGE("request start output stream error");
        return;
    }

//    AAudioStream_waitForStateChange(outputStream, inputState, &nextState, 10000000);
//
//    if(nextState != AAUDIO_STREAM_STATE_STARTED)
//    {
//        LOGE("output stream start failed, state = %d", nextState);
//        return;
//    }

    result = AAudioStream_requestStart(inputStream);
    if(result != AAUDIO_OK)
    {
        LOGE("request start input stream error");
        return;
    }
    nextState = AAUDIO_STREAM_STATE_UNINITIALIZED;
//    AAudioStream_waitForStateChange(inputStream, inputState, &nextState, 10000000);
//    if(nextState != AAUDIO_STREAM_STATE_STARTED)
//    {
//        LOGE("input stream start failed, state = %d", nextState);
//        return;
//    }

}

void AAudioEcho::stop() {
    aaudio_result_t result;
    aaudio_stream_state_t inputState = AAUDIO_STREAM_STATE_STOPPING;
    aaudio_stream_state_t nextState = AAUDIO_STREAM_STATE_UNINITIALIZED;
    if(inputStream)
    {
        result = AAudioStream_requestStop(inputStream);
        if(result != AAUDIO_OK)
        {
            LOGE("request input stream stop failed");
        } else
        {
            AAudioStream_waitForStateChange(inputStream, inputState, &nextState, 0);
            if(nextState != AAUDIO_STREAM_STATE_STARTED)
            {
                LOGE("input stream stop failed, state = %d", nextState);
            }
        }
    }
    nextState = AAUDIO_STREAM_STATE_UNINITIALIZED;
    if(outputStream)
    {
        result = AAudioStream_requestStop(outputStream);
        if(result != AAUDIO_OK)
        {
            LOGE("request output stream stop failed");
        } else
        {
            AAudioStream_waitForStateChange(outputStream, inputState, &nextState, 0);
            if(nextState != AAUDIO_STREAM_STATE_STARTED)
            {
                LOGE("output stream stop failed");
            }
        }
    }

}