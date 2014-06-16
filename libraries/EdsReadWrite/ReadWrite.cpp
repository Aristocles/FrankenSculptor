/*
  ReadWrite.h - Library for Read/Write of numbers to EEPROM
  Created by Eddy Jay, July 20, 2013.
  For use with FrankenScultor Project
*/

#include "Arduino.h" // Must include the standard arduino functions etc
#include "ReadWrite.h" // Add the header file we made

ReadWrite::ReadWrite() {
  intWriteCounter = 0;
}

unsigned long ReadWrite::getNumToRestore() {
  unsigned long numtorestore = 0;
  unsigned long num = 0;
  unsigned long restoreBytes[] = {0, 0, 0, 0};
  for (int i = 3; i >= 0; i--) {
    num = EEPROM.read(i);
    num = num << (8 * i);
//    num = restoreBytes[i] << (8 * i);
    numtorestore += num;
  }
  //Serial.print("Number to restore is "); Serial.println(numtorestore);
  //Serial.println();
  return numtorestore;
}



// Below is the actual code for the library (ie. class)
void ReadWrite::writeLong(int startPosition, unsigned long numtosave) {
  unsigned long temp = 0;
  int counter = 0;  
  unsigned long saveBytes[] = {0, 0, 0, 0};
  do {
    counter = 0;
    do {
      temp = (numtosave >> (8 * (counter + 1)));
      counter++;
    } while (temp > 255);
    numtosave = (numtosave - (temp << (8 * counter))); 
    saveBytes[counter] = temp;
    } while (counter > 1);
  saveBytes[0] = numtosave;
  Serial.print("Writing to EEPROM starting at byte position "); Serial.print(startPosition); Serial.print(" >");
  if (intWriteCounter % 5 == 0) {
    for (int i = 0; i < 4; i++) {
      Serial.print(", byte["); Serial.print(i); Serial.print("] = "); Serial.print(saveBytes[i]);
      EEPROM.write((startPosition + i), saveBytes[i]);
    }
  }
  intWriteCounter++;
  Serial.println();
}

/*
unsigned long ReadWrite::readLong(int startPosition) {
  //unsigned long saveBytes[] = {byte1, byte2, byte3, byte4};
  unsigned long numtorestore = 0;
  unsigned long num = 0;
  for (int i = 3; i >= 0; i--) {
    saveBytes[i] = EEPROM.read(i + 1);
    num = saveBytes[i] << (8 * i);
    numtorestore = numtorestore + num;
  }
  Serial.print("Reading from EEPROM < "); Serial.println(numtorestore);
  return numtorestore;
}
*/


