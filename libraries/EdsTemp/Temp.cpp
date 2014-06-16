/*
  Temp.h - Library for providing temperatures from sensor
  Created by Eddy Jay, July 20, 2013.
  For use with FrankenScultor Project
*/

#include "Arduino.h" // Must include the standard arduino functions etc
#include "Temp.h" // Add the header file we made

Temp::Temp(int intPin) {
  pinMode(intPin, OUTPUT);
  _intPin = intPin; // Adding underscore (_) to the beginning of a variable
					// is convention to show that the variable is private
  int _intTst = 0;
  int _intTst2 = 0;
  int _intTstDirection = 0;
}

// Below is the actual code for the library (ie. class)
double Temp::Thermister(int RawADC) {
  // See http://en.wikipedia.org/wiki/Thermistor for explanation of formula
  double temp = log(((10240000/RawADC) - 10000));
  temp = 1 / (0.001129148 + (0.000234125 * temp) + (0.0000000876741 * temp * temp * temp));
  temp = temp - 273.15;           // Convert Kelvin to Celcius
  return temp;
}

double Temp::getTemp() {
  double temp = Thermister(analogRead(_intPin));  // Read sensor
  Serial.print("getTemp()                               "); Serial.print(temp); Serial.println("C");
  return temp;
}

double Temp::getTemp(int intTstOffset, int intTstRange) {
  /*
    This is for testing the device.
    intTstOffset is the lowest temp to go to. It is represented as the number of degress offset from 0degC.
    A good offset is -10
    intTstRange is the maximum number of degrees above the offset to go. A range of 20 and offset of 10
    means the temp starts at -10 and goes up to +10   
  */
  double temp = Thermister(analogRead(_intPin));  // Read sensor
  Serial.print("getTemp(testing)                        "); Serial.print(temp); Serial.println("C");
  _intTst = (_intTst2 % (intTstRange + 1)) + (intTstOffset);
 if (_intTst == (intTstOffset + intTstRange)) // Control whether temp raises or lowers
    _intTstDirection = 0;
  if (_intTst == intTstOffset)
    _intTstDirection = 1;

  if (_intTstDirection == 0) {
    _intTst2--;
    Serial.println(_intTst);
    return(_intTst);
  }
  if (_intTstDirection == 1) {
    _intTst2++;
    Serial.println(_intTst);
    return(_intTst);
  }
  Serial.println(_intTst);
}
  
String Temp::checkTemp(int intLowTemp, int intHighTemp, double dblCurrentTemp, String strPrevStatus) {
   Serial.print("checkTemp(Lo/"); Serial.print(intLowTemp); Serial.print(", Hi/"); Serial.print(intHighTemp);
  Serial.print(", Curr/"); Serial.print(dblCurrentTemp); Serial.print(")      ");
  if (dblCurrentTemp >= intHighTemp) {
    Serial.println("'cool'");
    return ("cool");
    } else if (dblCurrentTemp <= intLowTemp) {
      Serial.println("'heat'");
      return ("heat");
    } else if (dblCurrentTemp < intHighTemp && dblCurrentTemp > intLowTemp) {
      return (strPrevStatus);
    } else {
      Serial.println("'Temp Sensor'");
      return ("Temp Sensor");
   }
}