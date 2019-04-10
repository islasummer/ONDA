#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <cstdlib>
#include <array>
using namespace std;

class readpcm
{
private:
public:
  double dat[400000];

   void read(const char* path){
     fstream f_in;
     short speech, value[400000];

     f_in.open (path, ios::in | ios::binary);

     int i = 0;
     while (i < 400000) {
       f_in.read((char *)&speech, 2);
                  value[i] = speech;
       //cout << speech << std::endl;
       i++;
     }

   //
     //double dat[i];

   //  for (i = 86000; i < 94000; i++)
     for (i = 0; i < 8192; i++)
     {
       dat[i] = value[i];
     }
   //

     ofstream audiod;
     audiod.open ("testpcm2.txt");
     for (i = 0; i < 8000; i++) {
       audiod << left << setw(10) << i << setw(15) << dat[i] << endl;
     }

   }

};
