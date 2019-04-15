/**
* @file hw_setup_record
* @brief defines hardware parameters and writes them to PCM device, samples PCM audio into buffer, returns buffer
* @author songwongtp, Niall Marshall
* @date 07/04/2019

*/


/*! @brief Include appropriate libraries and headers*/

#include "wav_header_working.h"
#include <iostream>
#include <cmath>
#include <utility>
#include <exception>

using namespace std;


constexpr int max_num_of_frames = 8192; //!<Maximum number of frames to store in buffer (for mono audio, 1 frame = 1 sample, for stereo, 1 frame = 2 samples

/*! @brief Define static members (parameters must be supported by PCM device)*/
uint16_t waveRecorder::number_of_channels = 1;	//!<Number of audio channels 
uint32_t waveRecorder::sample_rate = 44100; 	//!<Sampling rate 
uint16_t waveRecorder::bits_per_sample = 16;	//!<Data size of samples 
uint32_t waveRecorder::bytes_per_second = sample_rate * number_of_channels * bits_per_sample / 8;	//!<Bytes per second 
uint16_t waveRecorder::bytes_per_frame = number_of_channels * bits_per_sample / 8; //!<Bytes per frame
auto multiplier =  waveRecorder::bits_per_sample / 8 * waveRecorder::number_of_channels; //!<Multiplier for buffer



//std::pair<int, char *> waveRecorder::recordWAV(){
    
/**
* @brief samplePCM is called from the class PCMinitialiser writes hardware parameters to the declared PCM device, using ALSA library functions, then the PCM device is read and the data stored in a buffer
* Each ALSA function will be described briefly, further information can be found here:https://www.alsa-project.org/alsa-doc/alsa-lib/group___p_c_m___h_w___params.html
*/

char * waveRecorder::recordWAV(){

    int err; //!<Error code for ALSA functions

/**
* @brief Defines PCM hande
*/
    snd_pcm_t *handle;
/**
* @brief PCM hardware configuration space container
*/
    snd_pcm_hw_params_t *params;

    unsigned int sampleRate = sample_rate;	//!<Setting sample rate from header equal to local sample rate
    int dir;	//!<Sub unit direction
    snd_pcm_uframes_t frames = 1;	//!<number of frames to fill buffer (used in read function below)
    char *device = (char*) "plughw:0,0";	//!<Name of PCM device, can be found by typing aplay -l into command line, (1st digit is 'card number', 2nd digit is 'device number)

	/**
* @brief Open PCM device for audio capture, check error codes and throw exception if cannot open 
*/

    err = snd_pcm_open(&handle, device, SND_PCM_STREAM_CAPTURE, 0);
    if (err)
    {
        fprintf(stderr, "Unable to open PCM device: %s\n", snd_strerror(err));
        throw std::exception();
    }


	/**
* @brief Allocate a hardware parameters object 
*/
    snd_pcm_hw_params_alloca(&params);

/**
* @brief Fill it in with default values
*/
    snd_pcm_hw_params_any(handle, params);

/**
 * @brief Set the desired hardware parameters
 */

    /**
 * Interleaved mode (irrelevant for mono audio sampling), check error codes and throw exception if cannot set
 */
    err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err)
    {
        fprintf(stderr, "Error setting interleaved mode: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw std::exception();
    }
    /**
     * @brief Signed 16-bit little-endian format, check error codes and throw exception if cannot set
     */
    if (bits_per_sample == 16) err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    else err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_U8);
    if (err)
    {
        fprintf(stderr, "Error setting format: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw std::exception();
    }
    /**
	* @brief One or two channels (mono or stereo), defined in static members, check error codes and throw exception if cannot set 
        */
    err = snd_pcm_hw_params_set_channels(handle, params, number_of_channels);
    if (err)
    {
        fprintf(stderr, "Error setting channels: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw std::exception();
    }
    /**
	* @brief Set sampling rate, defined above, check error codes and throw exception if cannot set
	*/
    sampleRate = sample_rate;
    err = snd_pcm_hw_params_set_rate_near(handle, params, &sampleRate, &dir);
    if (err)
    {
        fprintf(stderr, "Error setting sampling rate (%d): %s\n", sampleRate, snd_strerror(err));
        snd_pcm_close(handle);
        throw std::exception();
    }
    sample_rate = sampleRate;
    /**
	* @brief Set period size, check error codes and throw exception if cannot set
	*/
    err = snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
    if (err)
    {
        fprintf(stderr, "Error setting period size: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw std::exception();
    }
    /**
	* @brief Write the parameters to the driver, check error codes and throw exception if cannot set
	*/
    err = snd_pcm_hw_params(handle, params);
    if (err < 0)
    {
        fprintf(stderr, "Unable to set HW parameters: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw  std::exception();
    }

    /**
	* @brief Use a buffer large enough to hold one period, check error codes and throw exception if cannot find
	*/
    err = snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    if (err)
    {
        fprintf(stderr, "Error retrieving period size: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
        throw  std::exception();
    }

    /**
	* @brief Get period time, check error codes and throw exception if cannot get
	*/

    err = snd_pcm_hw_params_get_period_time(params, &sampleRate, &dir);
    if (err)
    {
        fprintf(stderr, "Error retrieving period time: %s\n", snd_strerror(err));
        snd_pcm_close(handle);
  //      free(buffer);
        throw  std::exception();
    }

   
    long num_read_frames = 0; //!<Number of frames read from PCM device
    
    /**
	* @brief snd_pcm_readi reads PCM data into buffer, until number of read frames reaches the maximum number of read frames, check error codes and throw exception if cannot record to full length
	*/
           
	 while (num_read_frames < max_num_of_frames) {
                err = snd_pcm_readi(handle, buffer + num_read_frames * multiplier, frames);

                if (err > 0) {
                    num_read_frames += err;
                }
                if (err == -EPIPE) fprintf(stderr, "Overrun occurred: %d\n", err);
                if (err < 0) err = snd_pcm_recover(handle, err, 0);

                if (err < 0) {
                    fprintf(stderr, "Error occured while recording: %s\n", 		
		snd_strerror(err));
		snd_pcm_close(handle);
		throw std::exception();
            	}
         }	


    snd_pcm_drain(handle); //!<Stops the PCM preserving pending frames
    snd_pcm_close(handle); //!<Close PCM handle
    
	/**
 		@brief returns buffer to make it available for FFT
*/
      return buffer; 
}



waveRecorder::waveRecorder(){
  auto size = max_num_of_frames * multiplier; //!<Calculate size of buffer
      buffer = new char[size * 2];	      //!<Allocate memory to buffer
}
