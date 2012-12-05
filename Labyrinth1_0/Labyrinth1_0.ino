/*
Lee Sichello
200259098
CS 207
Project: Wii Labyrinth
Last update Nov 7, 2012

Notes:
  Wii Nunchuck Configuration:
      Data = pin A4
      CLK = pin A5
  Servo Wire Colors:
      Orange - Signal (pins 0,1)
      Red - 5V
      Brown - GND
  Solenoid Signal on pin 2
*/

#include<String.h>
#include <Servo.h> 
#include "Wire.h"
//#include "WiiChuckClass.h" //most likely its WiiChuck.h for the rest of us.
#include "WiiChuck.h"

#define MAXANGLE 90
#define MINANGLE -90

const int SRV1_CENTER = 80;
const int SRV2_CENTER = 80;

WiiChuck chuck = WiiChuck();
int angleStart, currentAngle;
int tillerStart = 0;
double angle;

int solenoid=2;
Servo servo1;  // create servo object to control a servo 
Servo servo2; // a maximum of eight servo objects can be created  
int srv1_pos = 90;    // variable to store the servo position 
int srv2_pos = 90;
int delta1=0;  // deltas represent difference between current servo angle and destination angle (one for each axis)
int delta2=0;

boolean useAccel=false;

///////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  //nunchuck_init();
  Serial.begin(115200);
  chuck.begin();
  chuck.update();
  //chuck.calibrateJoy();
  servo1.attach(0);  // attaches the servo on pin 9 to the servo object 
  servo2.attach(1);  // attaches the servo on pin 10 to the servo object 
  
  pinMode(solenoid, OUTPUT);
  digitalWrite(solenoid,LOW);
}

///////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  delay(20);
  chuck.update(); 
  
  //print nunchuck state to serial monitor
  PrintWiiChuck();
  
  //check if the C button on Nunchuck was pressed, if so, toggle between accelerometers and joystick
  if(chuck.buttonC){
   if (useAccel==true) 
     useAccel=false;
   else
    useAccel=true;
   delay(200); 
  }
  
  //check if the Z button on Nunchuck was pressed, if so, fire the solenoid
  if(chuck.buttonZ){
    digitalWrite(solenoid, HIGH);
    delay(20);
    digitalWrite(solenoid, LOW);
    delay(1000);
  }
  
  //if using the accelerometers find the diference between current accelrometer X angle and servo position 
  if(useAccel){
    delta1 = (chuck.readAccelX()*0.9 + SRV1_CENTER -srv1_pos)/5;  //divide by 5 to slowly approach the destination position
    delta2 = (chuck.readAccelY()*0.9 + SRV2_CENTER -srv2_pos)/5;
  }
   //else use the joystick to find the diference between current joystick X angle and servo position 
  else{
    delta1 = (chuck.readJoyX()*0.9 + SRV1_CENTER -srv1_pos)/8;  //divide by 8 to slowly approach the destination position
    delta2 = (chuck.readJoyY()*0.9 + SRV2_CENTER -srv2_pos)/8;
  }
  
  srv1_pos =srv1_pos + delta1;  //add difference to correct position
  srv2_pos =srv2_pos + delta2;
  servo1.write(srv1_pos);  //update servo position
  servo2.write(srv2_pos);
  
  //delay(0);
}

///////////////////////////////////////////////////////////////////////////////////////////
  //print nunchuck state to serial monitor
void PrintWiiChuck(){
  Serial.print("Roll:");
  Serial.print(format(chuck.readRoll()));
    Serial.print(", ");  
  Serial.print("Pitch:");
  Serial.print(format(chuck.readPitch()));
      Serial.print(", ");  

  Serial.print("AccelX:");
  Serial.print(format(chuck.readAccelX())); 
    Serial.print(", ");  
  Serial.print("AccelY:");
  Serial.print(format(chuck.readAccelY())); 
    Serial.print(", ");  
  Serial.print("AccelZ:");
   Serial.print(format(chuck.readAccelZ())); 
    Serial.print(", ");  

  Serial.print("JoyX:");  
  Serial.print(format(chuck.readJoyX()));
    Serial.print(", ");  
   Serial.print("JoyY:");  
  Serial.print(format(chuck.readJoyY()));
    Serial.print(", ");  

  if (chuck.buttonZ) {
     Serial.print("Z");
  } else  {
     Serial.print("-");
  }

    Serial.print(", ");  

//not a function//  if (chuck.buttonC()) {
  if (chuck.buttonC) {
     Serial.print("C");
  } else  {
     Serial.print("-");
  }
  Serial.println();
 
}

///////////////////////////////////////////////////////////////////////////////////////////
String format(const float inp){
 String sign,val;
 if(inp<0)
   sign="-";
 else
   sign=" ";
   
 int absol=abs(inp);
 if (absol<10)
   val="  "+String(absol);
 else if (absol<100 && absol>9)
   val=" "+String(absol);
 else
   val=String(absol);
   
 String outpt=sign+val;
 return outpt;
} 
