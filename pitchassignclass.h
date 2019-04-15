#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

class pitchassign
{
public:

  void assign(double x){
    if (81.19 <= x && x <= 83.63){
      cout << "E2\n";
    }
    else if (83.63 <= x && x <= 86.02){
      cout << "E2< >F2\n";
    }
    else if (86.02 <= x && x <= 88.6){
      cout << "F2\n";
    }
    else {
      cout << "Out of range\n";
    }
  }
};
