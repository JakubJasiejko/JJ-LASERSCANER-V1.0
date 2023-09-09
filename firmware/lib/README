/*
JJ TENSTESTER v1.0 by Jakub Jasiejko

gitHub link: https://github.com/JakubJasiejko/JJ-TENSTESTER-V1.0.git

august 2023

Used libraries: 
  - StepperDriver library by Laurentiu Badea: https://github.com/laurb9/StepperDriver/tree/master
  - HX711_ADC library by Olav Kallhovd: https://github.com/olkal/HX711_ADC
  - LCD library by Marco Schwartz: https://www.arduino.cc/reference/en/libraries/liquidcrystal-i2c/
Project licensed by MIT license

Firmware for main Arduino board, which works with stepper motors and loadCell. 
*/

#include <Arduino.h>

#include "SyncDriver.h"
#include <HX711_ADC.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  


//stepper motors and motion

#define steps 200   //amount of steps per one revolution stepper motor 
float oneStepAngle = 1.8;   //angle of one step stepper motor w/o gearbox

#define DIR1 7   //stepper driver pins
#define STEP1 6
#define EN1 8

#define DIR2 4
#define STEP2 5
#define EN2 12

#define V 300 //velocity
#define MS 1 //microsteping

BasicStepperDriver stepper(steps, DIR1, STEP1);   
BasicStepperDriver stepper1(steps, DIR2, STEP2);

SyncDriver controller(stepper,stepper1);

int pitch = 2;    //pitch of trapezoidal screw
float p = 13.73;    // stepper motor gear ratio


int v1 = 100; // velocity for slow test [mm/min]
int v2 = 300; //velocity for fast test [mm/min]


#define step 1.8    //angle of one step stepper motor w/o gearbox


float oneRevolution = 360 * p;  //angle needed to make one revolution stepper driver with gearbox

float numOfRot = 8;   //number of revolutions for fast and slow tests



float turn = oneRevolution * numOfRot;    //angle of revolution per numOfRot number of revolutions
float numOfSteps = turn / oneStepAngle;   //number of steps need for numOfRot revolutions


float numOfRotY = 2;    //number of revolutions for first part of Young Modulus test
float numOfRotY1 = 8;    //number of revolutions for second part of Young Modulus test


#define turnY oneRevolution * numOfRotY //angle of revolution for first part of Young modulus test
#define turnY1 oneRevolution * numOfRotY1 //angle of revolution for first part of Young modulus test

float numOfStepsY = turnY / oneStepAngle;   //number of steps need for first part of Young modulus test revolutions
float numOfStepsY1 = turnY1 / oneStepAngle;   //number of steps need for second part of Young modulus test revolutions

#define RPMJOG 300 //velocity for JOG movement
#define turnJOG oneRevolution * 2 //number of revolutions per one command sending for JOG

#define RPM1 ((v1*p)/(pitch))   //RPM for slow test and first part of Young modulus test
#define RPM2 ((v2*p)/(pitch))   //PRM for fast test and second part of Young modulus test

bool slow = false;
bool young = false;
bool fast = true;

float JOG = 0;

//loadcell

const int DTpin = 11;    //DOUT pin HX711 ADC
const int SCKpin = 13;   //SCK

HX711_ADC LoadCell(DTpin, SCKpin);  //initiallizing loadCell

unsigned long time = 0;
unsigned long currentTime = 0;

double lenghtPerOneRevolution = ((numOfRot * pitch)/numOfSteps); //needed for displacement calculation
double lenghtPerOneRevolutionYoungModulus = (((numOfRotY + numOfRotY1) * pitch)/(numOfStepsY + numOfStepsY1));

int z = 0;    
int l = 0;

//buttons

const int upButton = 2;   
const int downButton = 3;

  

int upButtonState = 0;     
int downButtonState = 0; 






void setup() {



lcd.init();         

lcd.backlight();
lcd.setCursor(5,0);
lcd.print("TENSTESTER"); 
lcd.setCursor(4, 1);
lcd.print("VERSION 1.0");
 lcd.setCursor(4,2);
lcd.print("POWER BY JJ");
lcd.setCursor(0,3);
lcd.print("ALL RIGHTS RESERVED");

delay(2000);
lcd.clear();

lcd.setCursor (2,0);
lcd.print("FOR USING CONNECT");
lcd.setCursor(7, 1);
lcd.print("DEVICE");
lcd.setCursor(4, 2);
lcd.print("TO COMPUTER");
lcd.setCursor(3, 3);
lcd.print("AND RUN MATLAB");
 
 delay(2000);
lcd.clear();

lcd.setCursor(6 , 1);

lcd.print("STARTING");

float calValue = -310.10; // calibration value

Serial.begin(9600);

//Serial.println("TENSTESTER V1.0");    //uncomment if you don't want to use matlab
//Serial.println("POWERED BY JJ");
//Serial.println("STARTING...");

pinMode(upButton, INPUT_PULLUP);     
pinMode(downButton, INPUT_PULLUP);


stepper.begin(V,MS);  //steppers innitializing
stepper1.begin(V,MS);

lcd.setCursor(14,1);

lcd.print(".");

LoadCell.begin();
 
  unsigned long stabTime = 2000; 
  boolean tare = true; 

  lcd.setCursor(15,1);
  lcd.print(".");

  LoadCell.start(stabTime, tare);
  if (LoadCell.getTareTimeoutFlag()) 
    {
      //Serial.println("CHECK LOADCELL WIRING!"); //uncomment if you don't want to use matlab or for diagnostics
      lcd.clear();
      lcd.print("CHECK LOADCELL WIRING");
      
      while (1);
    }
  else 
    {
      LoadCell.setCalFactor(calValue); 
    }

//Serial.println("READY");
lcd.setCursor(16,1);
lcd.print(".");
delay(200);

lcd.clear();
lcd.setCursor(3,1);
lcd.print("DEVICE READY!");
delay(2000);
lcd.clear();
}





void measurment() {
  static boolean newData = 0;
  
  
  if (LoadCell.update()) newData = true;
 
  if (newData) {
    
    
    if (millis() > time) {
      
      float measurment = LoadCell.getData();
      

            float displacement =lenghtPerOneRevolution * z;
            //Serial.print("MEASURMENT [N]");   //uncomment if you don't want to use matlab
            Serial.print(measurment , 6);
            Serial.print('\t');
            Serial.print(",");
            Serial.print('\t');
            //Serial.print("DISPLACEMENT [MM]");    //uncomment if you don't want to use matlab
            Serial.print(displacement , 6);
            Serial.println();

      newData = 0;
      time = millis();
      
  
    }
  
  }

}

void measurmentModulus() {
  static boolean newData = 0;
  
  
  if (LoadCell.update()) newData = true;
 
  if (newData) {
    
    
    if (millis() > time) {
      
      float measurment = LoadCell.getData();
      

            float displacement =lenghtPerOneRevolutionYoungModulus * l;
            //Serial.print("MEASURMENT [N]");   //uncomment if you don't want to use matlab
            Serial.print(measurment , 6);
            Serial.print('\t');
            Serial.print(",");
            Serial.print('\t');
            //Serial.print("DISPLACEMENT [MM]");    //uncomment if you don't want to use matlab
            Serial.print(displacement , 6);
            Serial.println();

      newData = 0;
      time = millis();
      
  
    }
  
  }

}
void modulusTest()

{
  controller.setRPM(RPM1);

  for(int i = 0; i <= (numOfStepsY); i++)
  { 
    controller.rotate(step , step);
    measurmentModulus();
    l++;                  
   }
    controller.setRPM(RPM2);

    for(int i = 0; i <= (numOfStepsY1); i++)
   { 
    controller.rotate(step , step);
    measurmentModulus();
    l++;              
   }

}
void Test()
{

  
  if(slow == true)
    {
      controller.setRPM(RPM1);
      

    }
  else if(fast == true)
    {
      controller.setRPM(RPM2);


    }

  for(int i = 0; i <= (numOfSteps); i++)
   { 
    controller.rotate(step , step);
    measurment();
    z++;
  }
            
  lcd.clear();
            
}

  
  void jog()
  {
  upButtonState = digitalRead(upButton);  
  downButtonState = digitalRead(downButton);

 
  if (upButtonState == LOW) 
    {
    
     
     
      controller.setRPM(RPMJOG);


      
      controller.rotate(360, 360);



    } 
  if (downButtonState == LOW)
    {

      controller.setRPM(RPMJOG);
     
      controller.rotate(-360, -360);



    }

  }

void loop() {
  
  upButtonState = digitalRead(upButton);  
  downButtonState = digitalRead(downButton);
 
 
 
 if(upButtonState == HIGH && downButtonState == HIGH)

{

 lcd.setCursor(4, 0);
 lcd.print("TO START TEST");
 lcd.setCursor(1, 1);
 lcd.print("CLICK MATLAB BUTTON");
 lcd.setCursor(3, 2);
 lcd.print("UP LEFT BUTTON");
 lcd.setCursor(2, 3);
 lcd.print("DOWN RIGHT BUTTON");
 
  String command = " ";


  if (Serial.available() > 0)
    {
    
    
    command = Serial.readStringUntil('\n');
 
      if (command == "M11") //test slow
      {
        //Serial.println("SLOW TEST STARTED");    //uncomment if you don't want to use matlab
      
        slow = true;

      lcd.clear();
      lcd.setCursor(1, 1);
      lcd.print("SLOW TEST STARTED");
      lcd.setCursor(5, 2);
      lcd.print("BE CAREFUL!");
        
        Test();

      lcd.clear();
      }
      else if (command == "M12") //test szybki
      {
       // Serial.println("FAST TEST STARTED");    //uncomment if you don't want to use matlab

        fast = true;

      lcd.clear();
      lcd.setCursor(1, 1);
      lcd.print("FAST TEST STARTED");
      lcd.setCursor(5, 2);
      lcd.print("BE CAREFUL!");

        Test();


      lcd.clear();
      }
      
      else if (command == "M13") //test modułu Younga
      
      {
       // Serial.println("YOUNG MODULUS TEST STARTED");   //uncomment if you don't want to use matlab
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("MODULUS TEST STARTED");
      lcd.setCursor(5, 2);
      lcd.print("BE CAREFUL!");
        
        modulusTest();

        lcd.clear();

      }
      

      else
      
      {
        //Serial.println ("UNKNOWN COMMAND");   //uncomment if you don't want to use matlab or for diagnostics
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print("UNKNOWN COMMAND!");
        delay(5000);
      }

    


      
      
      slow = false;
      fast = false;
      young = false;
      z = 0;
      l = 0;

      


}
}
else
  {
    jog();
  }
}
