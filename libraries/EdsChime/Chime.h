/*
  Chime.h - Library for playing different melodies on a speaker
  Created by Eddy Jay, July 20, 2013.
  For use with FrankenScultor Project
*/


#ifndef Chime_h // Wrapping the whole .h file up in this line..
#define Chime_h // and this line (along with the last line in the file -- #endif -- prevents any issues
				// if someone includes this library twice in their code

#include "Arduino.h"  // Must always have this. This includes all the types and constants inside the arduino language

class Chime
{
  public:
    Chime(int intPin); // Constructor of the class. This expects an int. Constructor has no return type
    void play1(int, int);
    void play2();
    void play3();
	void playWait();
  private:
    int _intPin;
};

#endif
