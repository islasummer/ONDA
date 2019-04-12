#include "wav_header_working.h"

int main(int argc, char *argv[])
{
    int err;
    waveRecorder recorder = waveRecorder();


    for (int j = 0; j < 10; ++j) {
        auto res = recorder.recordWAV();
    }


    free(res.second);
    return 0;
}
