#include "wav_header_working.h"

int main(int argc, char *argv[])
{
    int err;
    waveRecorder recorder = waveRecorder();

    err = recorder.recordWAV();

    if (err)
    {
        fprintf(stderr, "Error recording WAV file: %d\n", err);
        return err;
    }
    return 0;
}
