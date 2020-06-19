//
// Created by 祖国瑞 on 2020-06-06.
//

#ifndef AAUDIOTEST_AAUDIOECHO_H
#define AAUDIOTEST_AAUDIOECHO_H

#include <stdlib.h>
#include <iostream>
#include <android/log.h>
#include <aaudio/AAudio.h>

#define SAMPLE_COUNT 192
#define SAMPLE_RATE 48000

using namespace std;

class AAudioEcho {
public:
    AAudioEcho();
    ~AAudioEcho();
    void init();
    void destroy();
    void start();
    void stop();

private:
    AAudioStream *inputStream = NULL;
    AAudioStream *outputStream = NULL;

    static aaudio_data_callback_result_t audio_callback(AAudioStream *stream, void *userData, void *audioData, int32_t numFrames);

    int16_t *buffer = NULL;

};


#endif //AAUDIOTEST_AAUDIOECHO_H
