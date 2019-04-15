#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

class pitchdetect
{
public:
    double maxval;
    double freq;
    double freqhz;

  tuple<double, double> detect (double a[]) {

    int N = 8192;
    int i;

    for(i = N/2; i < N; i++)
    {
      a[i] = 0;
    }

    for(i = 0; i < 21; i++)    // For N = 32768 , i < 40  (50Hz at 38)
    {
      a[i] = 0;
    }

    for (i = 0; i < N; i++)
    {
      if( a[i] > maxval && i > 0 )   // Detect highest peak and therefore approximate fundamental frequency
      {
        maxval = a[i];
        freq = i;
      }
    }
     freqhz = freq * 44100 / N;

    //cout << "Maximum value is: " << maxval << endl << "Fundamental frequency is: " << freqhz << endl;

    return make_tuple(maxval, freqhz);
  }
};
