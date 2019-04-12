#include "wav_header_working.h"
#include <iostream>
#include <cmath>
#include <utility>
#include <exception>
using namespace std;

constexpr int max_num_of_frames = 8192;


//define static members
uint32_t waveRecorder::file_size = 0;
uint16_t waveRecorder::number_of_channels = 1;
uint32_t waveRecorder::sample_rate = 44100;
//uint32_t waveRecorder::sample_rate = 8001;
uint16_t waveRecorder::bits_per_sample = 16;
//uint16_t waveRecorder::bits_per_sample = 8;
uint32_t waveRecorder::bytes_per_second = sample_rate * number_of_channels * bits_per_sample / 8;
uint16_t waveRecorder::bytes_per_frame = number_of_channels * bits_per_sample / 8;


auto multiplier =  waveRecorder::bits_per_sample / 8 * waveRecorder::number_of_channels;


int binaryToDecimal(int n)
{
    int num = n;
    int dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;

        dec_value += last_digit * base;

        base = base * 2;
    }

    return dec_value;
}

std::pair<int, char *> waveRecorder::recordWAV(){
    //test initializing duration
    uint32_t duration = 0;
    //test dynamic file name
    std::string fileName, name = "combine-test", type = ".pcm";
    int part = 1;

    int err;
    int size;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int sampleRate = sample_rate;
    int dir;
    snd_pcm_uframes_t frames = 32;
    char *device = (char*) "plughw:0,1";
    char *buffer;
    int filedesc;

    printf("Capture device is %s\n", device);

    /* Open PCM device for recording (capture). */
    err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0);
    if (err)
    {
        fprintf(stderr, "Unable to open PCM device: %s\n", snd_strerror(err));
        throw std::exception();
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(handle, params);

    /* ### Set the desired hardware parameters. ### */

    /* Interleaved mode */
    err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err)
    {
        fprintf(stderr, "Error setting interleaved mode: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw std::exception();
    }
    /* Signed 16-bit little-endian format */
    if (bits_per_sample == 16) err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    else err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_U8);
    if (err)
    {
        fprintf(stderr, "Error setting format: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw std::exception();
    }
    /* Two channels (stereo) */
    err = snd_pcm_hw_params_set_channels(handle, params, number_of_channels);
    if (err)
    {
        fprintf(stderr, "Error setting channels: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw std::exception();
    }
    /* 44100 bits/second sampling rate (CD quality) */
    sampleRate = sample_rate;
    err = snd_pcm_hw_params_set_rate_near(handle, params, &sampleRate, &dir);
    if (err)
    {
        fprintf(stderr, "Error setting sampling rate (%d): %s\n", sampleRate, snd_strerror(err));
        snd_pcm_close(handle);
        throw std::exception();
    }
    sample_rate = sampleRate;
    /* Set period size*/
    err = snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
    if (err)
    {
        fprintf(stderr, "Error setting period size: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw std::exception();
    }
    /* Write the parameters to the driver */
    err = snd_pcm_hw_params(handle, params);
    if (err < 0)
    {
        fprintf(stderr, "Unable to set HW parameters: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw  std::exception();
    }

    /* Use a buffer large enough to hold one period */
    err = snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    if (err)
    {
        fprintf(stderr, "Error retrieving period size: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw  std::exception();
    }

    size = max_num_of_frames * multiplier; /* 2 bytes/sample, 2 channels */
    buffer = (char *) malloc(size);
    if (!buffer)
    {
        fprintf(stdout, "Buffer error.\n");
        snd_pcm_close(handle);
        throw std::exception();
    }

    err = snd_pcm_hw_params_get_period_time(params, &sampleRate, &dir);
    if (err)
    {
        fprintf(stderr, "Error retrieving period time: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        free(buffer);
        throw  std::exception();
    }

    fileName = name + std::to_string(part) + type;

    uint32_t pcm_data_size = sample_rate * bytes_per_frame * duration / 1000;
    file_size = pcm_data_size + 44 - 8;
    //file_size = pcm_data_size;

    filedesc = open(fileName.c_str(), O_WRONLY | O_CREAT, 0644);
    /*err = writeWAVHeader(filedesc);
    if (err)
    {
        fprintf(stderr, "Error writing .wav header.");
        snd_pcm_close(handle);
        free(buffer);
        close(filedesc);
        return err;
    }*/

    fprintf(stdout, "Channels: %d\n", number_of_channels);

    long num_read_frames = 0;
    for(duration = 0; duration < 1; duration ++){
        printf("___duration: %d\n", duration);
        //for(int i = ( (1000*1000) / (sample_rate / frames)); i > 0; i--)
        //{
            while (num_read_frames < max_num_of_frames) {
                err = snd_pcm_readi(handle, buffer + num_read_frames * multiplier, frames);

                if (err > 0) {
                    num_read_frames += err;
                }
                if (err == -EPIPE) fprintf(stderr, "Overrun occurred: %d\n", err);
                if (err < 0) err = snd_pcm_recover(handle, err, 0);

                if (err < 0) {
                    fprintf(stderr, "Error occured while recording: %s\n", snd_strerror(err));
                    snd_pcm_close(handle);
                    free(buffer);
                    close(filedesc);
                    throw std::exception();
                }
            }

            //write(filedesc, buffer, size);

            int buffdat = *buffer;
            //cout << binaryToDecimal(buffdat) << endl;
            //cout << buffdat << endl;
            //cout << sizeof(buffer) << endl << sizeof(*buffer) << endl << sizeof(&buffer) << endl;
        //}
    }


    close(filedesc);

    duration *= 1000;
    //Rewrite the header
    pcm_data_size = sample_rate * bytes_per_frame * duration / 1000;
    file_size = pcm_data_size + 44 - 8;
    //file_size = pcm_data_size;

    /*filedesc = open(fileName.c_str(), O_WRONLY | O_CREAT, 0644);
    err = writeWAVHeader(filedesc);
    close(filedesc);
    */

    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    //free(buffer);

    printf("Finished writing to %s\n", fileName.c_str());
    return {num_read_frames * multiplier, buffer};
}

waveRecorder::waveRecorder(){
    /*memcpy(&RIFF_marker, "RIFF", 4);
    memcpy(&filetype_header, "WAVE", 4);
    memcpy(&format_marker, "fmt ", 4);*/
}
