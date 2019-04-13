#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include "fftw3.h"
#include "pcmread.h"
#include "pitchdetectclass.h"
#include "record/wav_header_working.h"
#define N 8192
using namespace std;


int main(int argc, char *argv[])
 {

    //while(true) {

        fftw_complex *in = NULL, *out = NULL;    // Define the input and output array pointers
        fftw_plan plan;                             // Create a plan for the fft
        int i = 0;

// Allocate the input and output arrays
        in = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * N);
        out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * N);


// Load audio data from PCM file

/*
  readpcm pcm;
  pcm.read("/home/dan/Downloads/test-note-2.pcm");
*/

        //int i = 0;

        int err;
        waveRecorder recorder;
        try {
            recorder = waveRecorder();
        } catch(...) {
            std::cout << "Exception thrown!\n";
            return -1;
        }

        double vals[N];

        for (int j = 0; j < 1; ++j) {
            auto res = recorder.recordWAV();

            for (int j = 0; j < res.first / 2; j++) {
                short x = *(short *) (res.second + j * 2);
                //std::cout << x << std::endl;
                vals[j] = x;
                //std::cout << vals[j] << std::endl;
            }
            free(res.second);
        }


        for (i = 0; i < N; i++) {
            in[i][0] = vals[i];
            in[i][1] = 0;
        }


// Print input values into .txt file
        ofstream data;
        data.open("/home/dan/fftwtest.txt");
        for (i = 0; i < N; i++) {
            data << left << setw(5) << i << setw(15) << in[i][0] << setw(10) << in[i][1] << endl;
        }

        // Apply Hann window to the data
//
        for (int i = 0; i < N; i++) {
            double multiplier = 0.5 * (1 - cos(2 * M_PI * i / N));
            in[i][0] = multiplier * in[i][0];
        }
//

        // Complex 1d fft function
        plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        // Real input to complex output function below
        //p = fftw_plan_dft_r2c_1d(N, in[0], reinterpret_cast<fftw_complex*>(&out[0]), FFTW_ESTIMATE);

        // Execute the fft
        fftw_execute(plan);

        // Calculate complex output vector for plotting
        double y[i];
        //double maxval = 0;
        //double freq = 0;

        for (i = 0; i < N; i++) {
            double a = out[i][0];
            double b = out[i][1];
            y[i] = sqrt(a * a + b * b) / N;
        }

        // Retrieve maximum value and estimate fundamental frequency

        pitchdetect pitch;
        pitch.detect(y);

        cout << "Maximum value is: " << pitch.maxval << endl << "Fundamental frequency is: " << pitch.freqhz << endl;

        // Print output values into .txt file
        ofstream data2;
        data2.open("/home/dan/fftwtest2.txt");
        for (i = 0; i < N; i++) {
            data2 << left << setw(5) << i << setw(15) << abs(y[i]) << endl;
        }


        // Deallocate plan
        fftw_destroy_plan(plan);

        // Deallocate arrays
        fftw_free(in);
        fftw_free(out);
   // }

         return 0;
}
