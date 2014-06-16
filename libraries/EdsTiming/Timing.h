/*
  Timing.h - Library for providing Timingeratures from sensor
  Created by Eddy Jay, July 20, 2013.
  For use with FrankenScultor Project
*/


#ifndef Timing_h // Wrapping the whole .h file up in this line..
#define Timing_h // and this line (along with the last line in the file -- #endif -- prevents any issues
				// if someone includes this library twice in their code

#include "Arduino.h"  // Must always have this. This includes all the types and constants inside the arduino language
#include <Time.h>

class Timing
{
  public:
    Timing(long, unsigned long); // Constructor of the class. This expects an int. Constructor has no return type
	void setNewTime(long);
    int getHour();
	int getMinute();
	int getSecond();
	long getTimeLeft();
  private:
    long timeLeft; // session length in seconds. Usually 3600 (1 hour)
	unsigned long resetMillis;
};

#endif
