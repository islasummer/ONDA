#include "AudioFile.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
using namespace std;


int main()
 {
   AudioFile<double> audioFile;

   audioFile.load ("/home/dan/Documents/test.wav");

   audioFile.setBitDepth (16);
   audioFile.setSampleRate (8000);

   int numChannels = 1;
   int numSamples = 100000;

   audioFile.setAudioBufferSize (numChannels, numSamples);

   audioFile.printSummary();

   int channel = 0;
   numSamples = audioFile.getNumSamplesPerChannel();

   int i;
   double currentSample;

   for (i = 0; i < numSamples; i++)
   {
   	currentSample = audioFile.samples[channel][i];
   }

   ofstream audio;
   audio.open ("testwav.txt");
   for (i = 0; i < numSamples; i++) {
     audio << left << setw(10) << i << setw(15) << audioFile.samples[channel][i] << endl;
   }

   return 0;

 }
