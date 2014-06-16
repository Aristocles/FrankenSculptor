/*
  ReadWrite.h - Library for Read/Write of numbers to EEPROM
  Created by Eddy Jay, July 20, 2013.
  For use with FrankenScultor Project
*/


#ifndef ReadWrite_h // Wrapping the whole .h file up in this line..
#define ReadWrite_h // and this line (along with the last line in the file -- #endif -- prevents any issues
				// if someone includes this library twice in their code

#include "Arduino.h"  // Must always have this. This includes all the types and constants inside the arduino language
#include "EEPROM.h"

class ReadWrite
{
  public:
    // functions
    ReadWrite();
    void writeLong(int, unsigned long);
    //unsigned long readLong(int);
	unsigned long getNumToRestore();
    // unsigned long ReadWrite::readLong(int[] saveBytes);
    
    // variables
    //int writeLong(unsigned long); // Take a long and write to 4 bytes. Return the array of 4 bytes
    //unsigned long readLong(int[4]);    // Array os 4 ints is reference to the bytes on ROM. Return it as a number
  private:
    unsigned long temp;
    int counter;
    unsigned long saveBytes[];
	unsigned long numtorestore;
    unsigned long num;
    unsigned long restoreBytes[];
	int intWriteCounter;
};

#endif
