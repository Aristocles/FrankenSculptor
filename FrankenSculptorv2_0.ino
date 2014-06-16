/*
FrankenSculptor v1.0  14/Oct/2013  created by Eddy
Final release for general consumption. Still buggy but solid enough for use.
Custom libraries are required to be exported with this program.
Distribution of this code is permitted so long as this header comment is not modified.

Need to use following custom libraries
- Timing
- ReadWrite
- Temp
- Chime

Known Bugs:
- After selecting to restore (or not to) from a previous session the countdown timer begins immediately,
even though the user is prompted  for Hi and Lo temps and session hasnt begun [FIXED]
- Menu prompting code can be cleaned up
- The LCD screen only allows you to put up custom messages using strStatus (top right quadrant of screen),
so we cant use the entire screen for custom messages. LCDscreen library not implemented. Cant get this to
work just yet. Once I do then this bug wont be an issue anymore.
- The Serial.print code for the asking to restore a session does not take in to consideration an additional
zero that is needed for minutes/seconds below 10. This is just aesthetic as it doesnt effect funcionality
*/

#include <LiquidCrystal.h>
#include <Timing.h>
#include <ReadWrite.h>
#include <Temp.h>
#include <Chime.h>
#include <Time.h>
#include <math.h>
#include <EEPROM.h>

#define pinTempSensor 0
#define pinRelay 13
#define pinSpeaker 9
#define pinButLeft 10
#define pinButRight 12
#define pinButMid 11

// Global Variables
int intLogging = 1; // DEBUG = 1; NOTICE = 2.
Timing timer = Timing(3600, 1373155200); // 3600 seconds per session. 1373198400 set time since epoch
int intLowTemp = -3; // Initialise the low temp
int intHighTemp = -1; // Initialise the high temp
Chime chimer = Chime(pinSpeaker); // Chime is a class I wrote to control the speaker
LiquidCrystal lcd = LiquidCrystal(8, 7, 6, 5, 4, 3, 2); // Pins used for LCD screen
Temp temp = Temp(pinTempSensor); // Temp is a class I wrote to get temperature from sensor and other calculations
double dblCurrentTemp; // used to hold the current temp value
unsigned long time;
String strStatus = "cool"; // Device begins in cooling mode
int intWriteCounter = 0;
ReadWrite savefile = ReadWrite();
unsigned long restoretime = savefile.getNumToRestore();
int countrom = 0;

void setup(void) {
  //delay(3000); // Initial delay, giving enough time to open up serial console for debugging. Can remove this for production
  Serial.begin(9600); // Enable output to serial
  lcd.begin(16, 2);   // rows, columns.  use 16,2 for a 16x2 LCD, etc.
  pinMode(pinRelay, OUTPUT);
  pinMode(pinButLeft, INPUT);
  pinMode(pinButRight, INPUT);
  pinMode(pinButMid, INPUT);
  //  setTime(1262390400); // Set time to start at 00:00:00
  //  setTime(1262393995); // test time. This is plus 59 mins and 50 secs
  askLowTemp();
  askHighTemp();
  askToRestore();
  digitalWrite(pinRelay, HIGH); // turn relays on to run the machine
  Serial.print("Low temp set to: "); 
  Serial.println(intLowTemp);
  Serial.print("High temp set to: "); 
  Serial.println(intHighTemp);
} // End setup

void loop(void) {
  do {
    //int intButton = readButtons();
    delay(100);
    /*
      TEST MODE
     Adding ints in the getTemp function triggers a test function. The numbers are the offset and the range. See Temp.cpp
     example: temp.getTemp(-5, 10);
     This will cycle temps from -5 to +5 and keep looping.
     A getTemp() without passing ints will not run in test mode
     */
    double dblCurrentTemp = temp.getTemp();
    // double dblCurrentTemp = temp.getTemp(-10, 20); // Used for testing
    String strPrevStatus = strStatus; // Store previous status
    strStatus = temp.checkTemp(intLowTemp, intHighTemp, dblCurrentTemp, strPrevStatus); // Make sure the temp of the device is between LowTemp and HighTemp
    action(strPrevStatus, strStatus); // This will flip the relays if required
    long timeLeft = timer.getTimeLeft();
    drawScreen(strStatus, dblCurrentTemp, timeLeft);
    strStatus = checkComplete(timeLeft, strStatus); // supply minutes and seconds left in session
    countrom += 1;
    if (countrom % 10 == 0) // Modulo counter slows down writing to EEPROM
      savefile.writeLong(0, timeLeft);
  } 
  while (strStatus == "cool" || strStatus == "heat");  
  // This code only runs when halting or error
  drawError(strStatus);
} // End loop


// Other Functions

String checkComplete(long timeLeft, String strStatus) {
  if (timeLeft <= 0) {
    Serial.println("Cycle Complete. Halting"); 
    savefile.writeLong(0, 0); // Save time = 0. Because successfully completed session
    return("Cycle Complete");
  } // cycle has ended once we have hit 1 hour
  return(strStatus); // otherwise return the existing status, no state change
}

void drawError(String strError) {
  Serial.println("*******     HALTING or ERROR     *******");
  //  digitalWrite(pinRelay, LOW);
  int intOldLowTemp = intLowTemp; // Store temp settings
  int intOldHighTemp = intHighTemp;
  intLowTemp = 2;    // Change temp to something cold but not dangerous
  intHighTemp = 5;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HALTING!");
  lcd.setCursor(0,1);
  lcd.print(strError);
  if (strStatus.equals("Cycle Complete")) {
    //k4r
    chimer.play1(2, 30);
  } else if (strStatus.equals("Temp Sensor")) {
    digitalWrite(pinRelay, LOW); // Stop cooling while no temp is sensed
    chimer.play3();
    int count = 0;
    do {
      count += 1;
      if (count % 100 == 0) // Modulo counter slows down the chime
        chimer.play3();
      Serial.print("Waiting for temp sensor to come back.... dblCurrentTemp is: "); Serial.println(dblCurrentTemp);
    } while (isnan(temp.getTemp()) == true);
    Serial.println("TEMP SENSOR IS WORKING AGAIN... continuing...");
    intLowTemp = intOldLowTemp;
    intHighTemp = intOldHighTemp;
    strStatus = "heat";
  }
  // below two lines will wait infinitely for serial input before continuing with the program. So program will effectively end here.
  //while(Serial.available() == 0) { }  // There really is nothing between the {} braces
  //char x = Serial.read();
}

void action(String strPrevStatus, String strStatus) {
  if (strStatus == "cool" && strPrevStatus != "cool")
    cool();
  if (strStatus == "heat" && strPrevStatus != "heat")
    heat();
}

void cool() {
  Serial.println("cool()*********COOLING*************");
  digitalWrite(pinRelay, HIGH);
  analogWrite(A1, 0); // Disable heating LED, if enabled
  //  chimer.play2();
}

void heat() {
  Serial.println("heat()*********HEATING************");
  digitalWrite(pinRelay, LOW);
  analogWrite(A1, 255); // Using analog pin to light up LED (run out of digital pins)
  //  chimer.play3();
}

void drawScreen(String strStatus, double dblCurrentTemp, long timeLeft) { 
  // TOP LINE OF LCD   
  lcd.clear();
  lcd.setCursor(0, 0);
  if ((timeLeft / 60) < 10) // Prints the timer counting down
    lcd.print("0");
  lcd.print(timeLeft / 60);
  lcd.print(":");
  if ((timeLeft % 60) < 10)
    lcd.print("0");
  lcd.print(timeLeft % 60);

  lcd.setCursor(8,0);
  lcd.print(strStatus);
  lcd.setCursor(0,1);
  lcd.print(dblCurrentTemp);
  lcd.print("C");
}

int readButtons() {
  int butLeftState = digitalRead(pinButLeft);
  int butRightState = digitalRead(pinButRight);
  int butMidState = digitalRead(pinButMid);
  if (butLeftState == HIGH) {
    Serial.println("readButtons()                           Button pushed: LEFT");
    return 1;
  }
  if (butRightState == HIGH) {
    Serial.println("readButtons()                           Button pushed: RIGHT");
    return 3;
  }
  if (butMidState == HIGH) {
    Serial.println("readButtons()                           Button pushed: MIDDLE");
    return 2;
  }
}

void askToRestore() {
  if (restoretime > 20) {
    Serial.println("******************* MENU SELECT *******************");
    Serial.print("A possible unfinished session can be restored. Press the middle button to restore to ");
    if ((restoretime / 60) < 10 )
      Serial.print("0");
    Serial.print(restoretime / 60); 
    Serial.print(":");    
    if ((restoretime % 60) < 10)
      Serial.print("0");
    Serial.println(restoretime % 60);
    Serial.println("Any other button restarts");
    drawScreen("restore?", dblCurrentTemp, restoretime);

    boolean boolExit = false;    
    int intButton = -1;
    int count = 0;
    int count2 = 0;    
    do {
      count += 1;
      if (count % 50000 == 0) { // Modulo counter slows down printing of the dot below
        Serial.print(".");
        count2 += 1;
        if (count2 % 3 == 0) // Modulo counter slows down ever further, audio prompting for input
          chimer.playWait();
      }
      intButton = readButtons();
      if (intButton == 1 | intButton == 3) { // LEFT | RIGHT
        Serial.print("Restore time found in EEPROM of "); 
        Serial.print(restoretime / 60); 
        Serial.print(":");
        Serial.print(restoretime % 60); 
        Serial.println(" is being RESET");
        restoretime = 0; 
        timer.setNewTime(3600); // Start from the beginning
        boolExit = true;
      }
      if (intButton == 2) { // MIDDLE
        Serial.print("Restore time found in EEPROM of "); 
        Serial.print(restoretime / 60); 
        Serial.print(":");
        Serial.print(restoretime % 60); 
        Serial.println(" is being RESTORED");
        timer.setNewTime(restoretime);
        //timer.setNewTime(45); // Testing with leaving only 45 seconds left of session
        boolExit = true;
      }
    } 
    while (boolExit == false); 
  } 
}

void askLowTemp() {
  delay(300);
  Serial.println("******************* MENU SELECT *******************");
  Serial.println("Select lower threshold temperature");
  double dblLowTemp = intLowTemp;
  drawScreen("temp lo?", dblLowTemp, restoretime);

  boolean boolExit = false;    
  int intButton = -1;
  int count = 0;
  int count2 = 0;  
  do {
    count += 1;
    if (count % 50000 == 0) { // Modulo counter slows down printing of the dot below
      Serial.print(".");
      count2 += 1;
      if (count2 % 3 == 0) // Modulo counter slows down ever further, audio prompting for input
        chimer.playWait();
    }
    intButton = readButtons();
    if (intButton == 1) { // LEFT button
      Serial.print("Reducing temp by 1 to "); 
      Serial.println(dblLowTemp);
      delay(300);
      if (dblLowTemp > -8) // Dont allow to go below -8
          dblLowTemp = dblLowTemp - 1;
      drawScreen("temp lo?", dblLowTemp, restoretime);
    }
    if (intButton == 3) { // RIGHT button
      Serial.print("Increasing temp by 1 to "); 
      Serial.println(dblLowTemp);
      delay(300);
      if (dblLowTemp < 10) // Dont allow to go above 10        
          dblLowTemp = dblLowTemp + 1;
      drawScreen("temp lo?", dblLowTemp, restoretime);
    }
    if (intButton == 2) { // MIDDLE button
      Serial.print("Low temp set to "); 
      Serial.print(dblLowTemp); 
      Serial.println("C");
      delay(300);
      intLowTemp = dblLowTemp;
      boolExit = true;
    }
  } 
  while (boolExit == false);
}

void askHighTemp() {
  delay(300);
  Serial.println("******************* MENU SELECT *******************");
  Serial.println("Select higher threshold temperature");
  double dblHighTemp = intHighTemp;
  if (dblHighTemp <= intLowTemp)
    dblHighTemp = intLowTemp;
  drawScreen("temp hi?", dblHighTemp, restoretime);

  boolean boolExit = false;    
  int intButton = -1;
  int count = 0;
  int count2 = 0;  
  do {
    count += 1;
    if (count % 50000 == 0) { // Modulo counter slows down printing of the dot below
      Serial.print(".");
      count2 += 1;
      if (count2 % 3 == 0) // Modulo counter slows down ever further, audio prompting for input
        chimer.playWait();
    }
    intButton = readButtons();
    if (intButton == 1) { // LEFT button
      Serial.print("Reducing temp by 1 to "); 
      Serial.println(dblHighTemp);
      delay(300);
      if (dblHighTemp > intLowTemp) // Only allow to go as low as Low Temp
          dblHighTemp = dblHighTemp - 1;
      drawScreen("temp hi?", dblHighTemp, restoretime);
    }
    if (intButton == 3) { // RIGHT button
      Serial.print("Increasing temp by 1 to "); 
      Serial.println(dblHighTemp);
      delay(300);        
      if (dblHighTemp < 10) // Dont allow to go above 10 
          dblHighTemp = dblHighTemp + 1;
      drawScreen("temp hi?", dblHighTemp, restoretime);
    }
    if (intButton == 2) { // MIDDLE button
      Serial.print("High temp set to "); 
      Serial.print(dblHighTemp); 
      Serial.println("C");
      delay(300);
      intHighTemp = dblHighTemp;
      boolExit = true;
    }
  } 
  while (boolExit == false);
}

void logging(String strMsg1, int intType) {
  logging(strMsg1, "", "", intType);
}

void logging(String strMsg1, String strMsg2, int intType) {
  logging(strMsg1, strMsg2, "", intType);
}

void logging(String strMsg1, String strMsg2, String strMsg3, int intType) {
  /* Types:
  1 = DEBUG
  2 = NOTICE
  */
  if ((intLogging == 1) && (intType == 1 || intType == 2)) {
    
  }
  
  
}


