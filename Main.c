/*
 TITLE: Blind openning device
 VERSION: 1.0
 CREATED: 08.10.19
 AUTHOR: GREER, J
  
 LAST MODIFIED: Nil.
 
 REFERENCES:
 Chaudhary, K. K. (2018, August 22). Arduino Digital Clock using DS3231 Pi Module. 
    Retrieved October 8, 2019, from https://bestengineeringprojects.com/arduino-digital-clock-using-ds3231-pi-module/.

  DESCRIPTION:
  Keeps track of the current time (via a DS3231 module) and activates a motor when a preset target time is reached.

 DEPENDANCIES:
  - Wire module

 CIRCUIT:
  - DS3231 module
      - 3.3v to '+' pin
      - Gnd to '-' pin
      - A4 to 'C' pin
      - A5 to 'D' pin
  - Motor controller board
    -
    -
    -
    -

 NOTE: Search for " *! " to see all the places that require input
 */

//Modules
#include <Wire.h>

//Variables
bool blindStatus = 0; // 0 is closed, 1 is open


//Pins
#define motor 11 //*! -- Change the motor pin here
#define buttonPin = 1 //*! -- Change the button pin here // I'm thinking of putting in a button that will open / close the blinds

//Assign memory
#define DS3231_I2C_ADDRESS 0x68


void setup() {
  Wire.begin();
  //Setting up modules for use
  Serial.begin(9600); //Start serial for diagnostic

  //Pin settup
  pinMode(motor, OUTPUT);
  
 //-------------------------------------------------------------------------
// //*! -- Follow the instructions below to adjust the time

  // Uncomment to adjust. Be sure to // back once fixed.
 //DS3231 seconds, minutes, hours, day, date, month, year
 //setDS3231time(00,30,00,3,8,10,19);
//--------------------------------------------------------------------------

  delay(2000); // Delay (2s) just to let everything get caught up / give you some breathing room
}

//Supporting functions

byte decToBcd(byte val) // Convert normal decimal numbers to binary coded decimal
{
  return( (val/10*16) + (val%10) );
}


byte bcdToDec(byte val) // Convert binary coded decimal to normal decimal numbers
{
  return( (val/16*10) + (val%16) );
}


                                        //Set time for module according to values above
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)     
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

                                //Read time from module
void readDS3231time(
byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}



//Custom functions
void openBlinds(){
  //Checks to see if blinds are open; if open - DO NOTHING; if closed - Run motor forward for an amount of time  
  if(!blindStatus){
    //*! -- This code has yet to be written
    //Run motor
    blindStatus = 1;

    //*! -- Also these lines are just for testing and can be removed
    Serial.println("BLINDS Openned, blind status changed to 1");
  }
}

void closeBlinds(){
  //Checks to see if blinds are closed; if closed - DO NOTHING; if open - Run motor backward for an amount of time
  if(blindStatus){
    //*! -- This code has yet to be written
    //Run motor
    
    blindStatus = 0;
    Serial.println("CLOSED blinds activated ");
    Serial.print(blindStatus);
  }
  }


void displayTime(
  byte *second,
  byte *minute,
  byte *hour,
  byte *dayOfWeek,
  byte *dayOfMonth,
  byte *month,
  byte *year){
    
  if (*hour == 0){
    Serial.print("0");
  }
  Serial.print(*hour, DEC);
  
  Serial.print(":");
  
  if (*minute < 10){
    Serial.print("0");
  }
  Serial.print(*minute, DEC);

  Serial.print(":");

  if (*second < 10){
    Serial.print("0");
  }
  Serial.print(*second, DEC);

  Serial.print(" -- ");

  if (*dayOfMonth < 10){
    Serial.print("0");
  }
  Serial.print(*dayOfMonth, DEC);

  Serial.print(".");

   if (month < 10){
    Serial.print("0");
  }
  Serial.print(*month, DEC);

  Serial.print(".");

  if (*year < 10){
    Serial.print("0");
  }
  Serial.println(*year);
   
  }

void loop() {
  //Retreive time
   byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;  //Create variables for time to be put into
  
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year); // retrieve data from DS3231

  //Display time
  displayTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year); 

  //Check time against presets
   if( hour == 0 && minute == 16 && second == 00) //*! -- Change the time blinds will open here
   {
    openBlinds();}

   else if ( hour == 0 && minute == 17 && second == 00) //*! -- Change the time blinds will close here
   {
    closeBlinds();}

  //*!NOTE - to add more alarms simply put "else if( hour == 0 && minute == 7 && second == 00){}" above this line and put the action in the "{}" ** make sure to change the time**
  
  //Delay
  delay(1000);
    
  }
