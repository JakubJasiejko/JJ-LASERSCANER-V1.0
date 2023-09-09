//dodaj w void scan przeliczanie wartosci predkosci

#include <Arduino.h>
#include "A4988.h"
#include "SyncDriver.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  

//steppers paramerers
#define kroki 200

#define DIR1 7
#define STEP1 6
#define EN1 8

#define DIR2 4
#define STEP2 5
#define EN2 12

int V = 100;
#define MS 1 //microstep

#define X 1917.19745 //kąt obrotu w osi X
#define Y 150 //kąt obrotu w osi y

int feedY = 150; 

//homing
#define homeS 360
#define Yhome 180
#define Vhome 200

#define Xlimit 13
#define Ylimit 12

int XlimitState = 0;
int YlimitState = 0;

//initializing steppers
BasicStepperDriver stepper(kroki, DIR1, STEP1);
BasicStepperDriver stepper1(kroki, DIR2, STEP2);

SyncDriver controller(stepper,stepper1);

//butonns
#define motionButton 1
#define scanButton 2
#define homeButton 3

const int buttonAddPin = 9;       // Pin przycisku do dodawania
const int buttonSubtractPin = 10;  // Pin przycisku do odejmowania
const int buttonMenuPin = 9;      // Pin przycisku menu (zmieniony na 9)


int scanButtonState = 0;
int homeButtonState = 0;
int velocityMenuState = 0;
int motionButtonState = 0;


//velocity menu

bool showVScreen = false;     // Flaga do wyboru ekranu
bool showMScreen = false; 

void setup() {

  lcd.init();         

  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("LASERSCANER"); 
  lcd.setCursor(4, 1);
  lcd.print("VERSION 1.0");
  lcd.setCursor(4,2);
  lcd.print("POWER BY JJ");
  lcd.setCursor(0,3);
  lcd.print("ALL RIGHTS RESERVED");
  


  stepper.begin(V,MS);
  stepper1.begin(V,MS);

  pinMode(scanButton , INPUT_PULLUP);
  pinMode(homeButton , INPUT_PULLUP);
  pinMode(motionButton , INPUT_PULLUP);
  

  pinMode(Xlimit , INPUT_PULLUP);
  pinMode(Ylimit, INPUT_PULLUP);

  pinMode(buttonAddPin, INPUT_PULLUP);      // Ustawienie pinu przycisku dodawania jako wejście z rezystorem Pull-up
  pinMode(buttonSubtractPin, INPUT_PULLUP); // Ustawienie pinu przycisku odejmowania jako wejście z rezystorem Pull-up
  pinMode(buttonMenuPin, INPUT_PULLUP);     // Ustawienie pinu przycisku menu jako wejście z rezystorem Pull-up (zmieniony na 9)

  delay(3000);
  lcd.clear();

}
void scan ()
{
  
  int j = 0;

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("SCANING");

   controller.setRPM(V);
    for(j=0;j<=3;j++){
  
  
  controller.rotate(-X,-X);
  
  delay(1000);

  controller.rotate(Y,-Y);

  delay(1000);

  controller.rotate(-X,-X);

  delay(1000);
  
  

}
}

void home()
{
  
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HOMING");
  

XlimitState=digitalRead(Xlimit);
YlimitState = digitalRead(Ylimit);

for(int z = 0; z < 1;  z++) 
  {
    XlimitState=digitalRead(Xlimit);
    if(XlimitState == HIGH)
      {
        controller.setRPM(Vhome);
        controller.rotate(homeS, homeS);

      }


  }

}




void menu()
  {
    
    lcd.setCursor(0, 0);
    lcd.print("HOME PRESS RED");
    lcd.setCursor(0, 1);
    lcd.print("SCAN PRESS GREEN");
    lcd.setCursor(0, 2);
    lcd.print("VELOCITY PRESS BLUE");
    lcd.setCursor(0, 3);
    lcd.print("MOTION PRESS BLUE");
  }



void velocity()
{

  while(digitalRead(buttonMenuPin == LOW))
  {
  
  // Odczyt stanów przycisków
  int buttonAddState = digitalRead(buttonAddPin);
  int buttonSubtractState = digitalRead(buttonSubtractPin);
  int buttonMenuState = digitalRead(buttonMenuPin);

  // Obsługa przycisku menu
  if (buttonMenuState == HIGH) { // Zmieniony na HIGH
    showVScreen = !showVScreen; // Przełącz flagę między ekranami
    delay(200);
    lcd.clear();                         // Wyczyść ekran
    if (showVScreen) {
      // Jeśli flaga jest ustawiona na true, przejdź do ekranu z wartością
      lcd.setCursor(0 ,0);
      lcd.print("SET VELOCITY");
      lcd.setCursor(0, 1);
      lcd.print("CLICK + OR -");
      lcd.setCursor(0, 2);
      lcd.print("VELOCITY: ");
      lcd.setCursor(10, 2);
      lcd.print(V);
      lcd.setCursor(0,3);
      lcd.print("SUBMIT BY OK");
  
    } else {
      // W przeciwnym razie wróć do ekranu menu
      loop();
      
    }
    delay(200);  // Opóźnienie anty-drebne

  }
  else

  // Obsługa przycisków dodawania i odejmowania (tylko na ekranie wartości)
  if (showVScreen) {
    if (buttonAddState == HIGH) {
      V += 10;  // Dodaj 10 do wartości
      if (V < 0) {
        V = 0;  // Ogranicznik: wartość nie może być mniejsza niż 0
      }
      lcd.setCursor(10, 2);  // Ustawienie kursora na pozycję wartości
      lcd.print("   ");     // Wymazanie poprzedniej wartości
      lcd.setCursor(10, 2);  // Ustawienie kursora na pozycję wartości
      lcd.print(V);     // Wyświetlenie nowej wartości
      delay(200);           // Opóźnienie anty-drebne
    }

    if (buttonSubtractState == HIGH) {
      V -= 10;  // Odejmij 10 od wartości
      if (V < 0) {
        V = 0;  // Ogranicznik: wartość nie może być mniejsza niż 0
      }
      lcd.setCursor(10, 2);  // Ustawienie kursora na pozycję wartości
      lcd.print("   ");     // Wymazanie poprzedniej wartości
      lcd.setCursor(10, 2);  // Ustawienie kursora na pozycję wartości
      lcd.print(V);     // Wyświetlenie nowej wartości
      delay(200);           // Opóźnienie anty-drebne
    }
  }

  
  }
  }


void loop() {
  
  while(1)
  {
  int scanButtonState = 0;
  
  scanButtonState = digitalRead(scanButton);
  homeButtonState = digitalRead(homeButton);
  velocityMenuState = digitalRead(buttonMenuPin);
  motionButtonState = digitalRead(motionButton); 
  menu();

  
  if(scanButtonState == HIGH)
    {
      scan();
      lcd.clear();
    }
  if(homeButtonState == HIGH)
  {
    home();
    lcd.clear();
  }
  if(velocityMenuState == HIGH)
    {
      velocity();
      lcd.clear();

    }

}
}

 



