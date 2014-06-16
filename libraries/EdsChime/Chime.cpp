/*
  Chime.h - Library for playing different melodies on a speaker
  Created by Eddy Jay, July 20, 2013.
  For use with FrankenScultor Project
*/

#include "Arduino.h" // Must include the standard arduino functions etc
#include "Chime.h" // Add the header file we made

Chime::Chime(int intPin) {
  pinMode(intPin, OUTPUT);
  _intPin = intPin; // Adding underscore (_) to the beginning of a variable
					// is convention to show that the variable is private
}

// Below is the actual code for the library (ie. class)
void Chime::play1(int intCount, int intDelay) {
  intDelay *= 1000; // Get milliseconds
  for (int x = 0; x < intCount; x++) {
    for (int i = 100; i < 3000; i = i + 100) {
      tone(_intPin, i, 400);
      delay(100);
    }
    for (int i = 3000; i > 100; i = i - 100) {
      tone(_intPin, i, 400);
      delay(100);
    }  
    delay(intDelay); // Wait
  }
}

void Chime::play2() {
 for (int freq = 300; freq <= 700; freq += 200) {  // Play triple tone
    tone(_intPin, freq); // play tone
    delay(100);
    noTone(_intPin); // stop playing tone
  }
}
  
  void Chime::play3() {
      for (int freq = 700; freq >= 300; freq -= 200) {  
    tone(_intPin, freq);
    delay(100);
    noTone(_intPin);
  }
}  
  
  void Chime::playWait() {
	// Short and sweet beep which prompts the user it is waiting for something
	tone(_intPin, 200);
	delay(50);
	tone(_intPin, 250);
	delay(100);
	noTone(_intPin);
}


