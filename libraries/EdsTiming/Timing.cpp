/*
  Timing.h - Library for providing Timing of sessions
  Created by Eddy Jay, July 20, 2013.
  For use with FrankenScultor Project
*/

#include "Arduino.h" // Must include the standard arduino functions etc
#include "Timing.h" // Add the header file we made
#include <Time.h>

Timing::Timing(long sessionLength, unsigned long timeSet) { // sessionLength is in seconds; timeSet is time since epoch
  timeLeft = sessionLength;
	//Serial.print("sessionLength is: "); Serial.println(sessionLength);
	//Serial.print("timeLeft in constructor is: "); Serial.println(timeLeft);
  setTime(timeSet); // Usually set to 1262390400 which starts hr:min:sec at 00:00:00
  //Serial.print("Time has been set to: "); Serial.print(day()); Serial.print("/"); Serial.print(month());
  //Serial.print("/");Serial.print(year()); Serial.print(" "); Serial.print(hour()); Serial.print(":");
  //Serial.print(minute()); Serial.print(":"); Serial.println(second());
  resetMillis = 0;
}

// Below is the actual code for the library (ie. class)
int Timing::getHour() {
	return (int)hour(); // casting return value as 
}

int Timing::getMinute() {
	return (int)minute();
}

int Timing::getSecond() {
	return (int)second();
}

long Timing::getTimeLeft() {
	//Serial.print("Millis is: "); Serial.println(millis());
	//Serial.print("timeLeft is: "); Serial.println(timeLeft);
	
	return (timeLeft - ((millis() - resetMillis) / 1000));
}

void Timing::setNewTime(long sessionLength) {
  timeLeft = sessionLength;
  resetMillis = millis();
}


/*
double Timing::getTiming() {
  double Timing = Thermister(analogRead(_intPin));  // Read sensor
  Serial.print("getTiming()                               "); Serial.print(Timing); Serial.println("C");
  return Timing;
}

double Timing::getTiming(int intTstOffset, int intTstRange) {
  double Timing = Thermister(analogRead(_intPin));  // Read sensor
  Serial.print("getTiming(testing)                        "); Serial.print(Timing); Serial.println("C");
  _intTst = (_intTst2 % (intTstRange + 1)) + (intTstOffset);
 if (_intTst == (intTstOffset + intTstRange)) // Control whether Timing raises or lowers
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
  
  String Timing::checkTiming(int intLowTiming, int intHighTiming, double dblCurrentTiming, String strPrevStatus) {
   Serial.print("checkTiming(Lo/"); Serial.print(intLowTiming); Serial.print(", Hi/"); Serial.print(intHighTiming);
  Serial.print(", Curr/"); Serial.print(dblCurrentTiming); Serial.print(")      ");
  if (dblCurrentTiming >= intHighTiming) {
    Serial.println("'cool'");
    return ("cool");
    } else if (dblCurrentTiming <= intLowTiming) {
      Serial.println("'heat'");
      return ("heat");
    } else if (dblCurrentTiming < intHighTiming && dblCurrentTiming > intLowTiming) {
      return (strPrevStatus);
    } else {
      Serial.println("'Timing Sensor'");
      return ("Timing Sensor");
   }
}
*/