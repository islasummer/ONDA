#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include "fftw3.h"
#define N 8192
using namespace std;


int main()
 {
  fftw_complex *in = NULL, *out = NULL;    // Define the input and output array pointers
  fftw_plan plan;                             // Create a plan for the fft
  int i;

// Allocate the input and output arrays
  in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

  // Generate complex sine wave array
  for (i = 0; i < N; i++) {
    in[i][0] = sin(440 * 2 * M_PI * i/N); //real
    in[i][1] = 0;                    //imaginary
  }

// Print input values into .txt file
 ofstream data;
 data.open ("fftwtest.txt");
  for (i = 0; i < N; i++) {
    data << left << setw(5) << i << setw(15) << in[i][0] << setw(10) << in[i][1] << endl;
  }

  // Complex 1d fft function
  plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  // Real input to complex output function below
  //p = fftw_plan_dft_r2c_1d(N, in[0], reinterpret_cast<fftw_complex*>(&out[0]), FFTW_ESTIMATE);

  // Execute the fft
  fftw_execute(plan);

  // Calculate complex output vector for plotting
  double y[i];
  for (i = 0; i < N; i++)
  {
    double a = out[i][0];
    double b = out[i][1];
    y[i] = sqrt(a*a+b*b) / N ;
  }

 // Print output values into .txt file
  ofstream data2;
  data2.open ("fftwtest2.txt");
    for (i = 0; i < N; i++) {
      data2 << left << setw(5) << i << setw(15) << abs(y[i]) << endl;
      }

  // Deallocate plan
  fftw_destroy_plan(plan);

  // Deallocate arrays
  fftw_free(in);
  fftw_free(out);

  return 0;
}
