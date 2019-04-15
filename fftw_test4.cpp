#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <record/wav_header_working.h>
#include <thread>
#include "fftw3.h"
//#include "pcmread.h"
#include "pitchdetectclass.h"
#include "displayclass.h"
#include "record/wav_header_working.h"
#define N 16384
//#include "pcmtoarrayclass.h"
#define N 8192
//#include "/home/pi/wiringPi/wiringPi/wiringPi.h"
//#include "/home/pi/wiringPi/devLib/lcd.h"
using namespace std;

/*
//USE WIRINGPI PIN NUMBERS
#define LCD_RS  25               //Register select pin
#define LCD_E   6                //Enable Pin
#define LCD_D4  23               //Data pin 4
#define LCD_D5  22               //Data pin 5
#define LCD_D6  21               //Data pin 6
#define LCD_D7  14               //Data pin 7
*/

//int main(int argc, char *argv[])
int main()
 {

  while(true){

//   usleep (100000);


  fftw_complex *in = NULL, *out = NULL;    // Define the input and output array pointers
  fftw_plan plan;                             // Create a plan for the fft
  //int i = 0;

// Allocate the input and output arrays
//  fftw_complex* in = new fftw_complex[N];

  //using cmplx = double[2];
  in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

  //in.reserve(N);

//  fftw_complex* in = new fftw_complex[N];

// Load audio data from PCM file

/*
  readpcm pcm;
  pcm.read("/home/dan/Downloads/test-note-2.pcm");
*/

//  int i = 0;

//    int err;
//
   waveRecorder recorder = waveRecorder();

   double vals[N];
//   short *x;
//   x = new short[N];

//    thread worker(recorder.recordWAV);

    for(int k = 0; k < 1; ++k) {
        auto res = recorder.recordWAV();


          //  for (int j = 0; j < res.first / 2; j++) {
            for (int j = 0; j < N; j++) {
            //short x = *(short *)(res.second + j * 2);
              short x = *(short *)(res + j * 2);
            //std::cout << x << std::endl;
            //std::cout << x << std::endl << j << std::endl;
              //std::cout << j << std::endl;

              vals[j] = x;
          //    std::cout << vals[j] << std::endl;

        }
      //  delete[] res;
  //      delete[] x;
    }
    //

//
//  usleep (100000);

  for (int i = 0; i < N ; i ++)
  {
    in[i][0] = vals[i];
    //std::cout << vals[i] << std::endl;
    in[i][1] = 0;

  //  cout  << in[i][0] << endl;
  }


// Print input values into .txt file
 ofstream data;
 data.open ("/home/pi/fftwtest.txt");
  for (int i = 0; i < N; i++) {
    data << left << setw(15) << i << setw(15) << in[i][0] << setw(10) << in[i][1] << endl;
  }

 // Apply Hann window to the data
//
  for (int i = 0; i < N; i++)
  {
    double multiplier = 0.5 * (1 - cos(2 * M_PI * i/N));
    in[i][0] = multiplier * in[i][0];

//    cout  << in[i][0] << endl;
//    cout  << i << endl;
  }
//

  // Complex 1d fft function
  plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  // Real input to complex output function below
  //p = fftw_plan_dft_r2c_1d(N, in[0], reinterpret_cast<fftw_complex*>(&out[0]), FFTW_ESTIMATE);

  // Execute the fft
  fftw_execute(plan);

/*
  for (int i = 0; i < N; i++){

    cout << out[i][0] << endl;
    cout << i << endl;
  }
*/

//


  // Calculate complex output vector for plotting
  //int i;
  double y[N];
  //double y[i] =0;
//  double maxval;
//  double freqhz;

  for (int i = 0; i < N; i++)
  {
  //  cout  << out[i][0] << endl;
    double a = out[i][0];
    double b = out[i][1];
    y[i] = sqrt(a*a+b*b) / N ;
//    cout  << y[i] << endl;
  }

 // Retrieve maximum value and estimate fundamental frequency

   pitchdetect pitch;
   pitch.detect(y);

   cout << "Maximum value is: " << pitch.maxval << endl << "Fundamental frequency is: " << pitch.freqhz << endl;

   //string freqLCD = to_string(pitch.freqhz);

//
  //  string f = to_string(pitch.freqhz);
/*
    static int lcdHandle;
    wiringPiSetup();
    lcdHandle = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

    lcdPuts (lcdHandle, "Frequency:") ;

//    lcdPrintf(lcdHandle, "%f", pitch.freqhz);
//    lcdClear(lcd);
//
//    lcd_string("F= " + str(freqLCD), 1);
*/

  // Print output values into .txt file
  ofstream data2;
  data2.open ("/home/pi/fftwtest2.txt");
    for (int i = 0; i < N; i++) {
      data2 << left << setw(15) << i << setw(15) << abs(y[i]) << endl;
    }
//
  //    cout << y[i] << endl;

  displaylcd disp;

  thread worker(disp.lcd, pitch.freqhz);

//  disp.lcd(pitch.freqhz);
//

//

  // Deallocate plan
  fftw_destroy_plan(plan);

  // Deallocate arrays
  fftw_free(in);
  fftw_free(out);

  //usleep (100000);
  worker.join();

 }

  return 0;
}
