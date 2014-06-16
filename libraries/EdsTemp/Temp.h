/*
  Temp.h - Library for providing temperatures from sensor
  Created by Eddy Jay, July 20, 2013.
  For use with FrankenScultor Project
*/


#ifndef Temp_h // Wrapping the whole .h file up in this line..
#define Temp_h // and this line (along with the last line in the file -- #endif -- prevents any issues
				// if someone includes this library twice in their code

#include "Arduino.h"  // Must always have this. This includes all the types and constants inside the arduino language

class Temp
{
  public:
    Temp(int intPin); // Constructor of the class. This expects an int. Constructor has no return type
    double Thermister(int);
    double getTemp();
    double getTemp(int, int);
    String checkTemp(int, int, double, String);   
  private:
    int _intPin;
    int _intTst;
    int _intTst2;
    int _intTstDirection;
};

#endif
