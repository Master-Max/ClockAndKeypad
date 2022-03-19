// includes
#include <LiquidCrystal.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Keypad.h>


LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'D','C','B','A'},
  {'#','9','6','3'},
  {'0','8','5','2'},
  {'*','7','4','1'}
};

byte rowPins[ROWS] = {A0, A1, A2, A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A4, A5, A6, A7}; //connect to the column pinouts of the keypad

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  Serial.begin(9600);
  while(!Serial);
  setSyncProvider(RTC.get);
  if(timeStatus()!= timeSet)
    Serial.println("Cant sync with RTC");
  else
    Serial.println("RTC has set the system time");
}

char customKey = '+';
//char ckLast = '';/
bool menuOn = false;
bool settingNT = false;
char newTime[7];
int ntIndex = 0;

void loop() {
  // put your main code here, to run repeatedly:
//  lcd.setCursor(0,1);
//  lcd.print();

  customKey = customKeypad.getKey();
  if(customKey){
    Serial.println(customKey);
    lcd.setCursor(0,1);
    lcd.print(customKey);
    menuOn = true;
    if(settingNT){
      newTime[ntIndex] = customKey;
      ntIndex++;

      if(ntIndex >= 6){
        newTime[6] = 0;
        Serial.println("NewTime Sub:");
        char hh[] = {newTime[0], newTime[1],0};
        Serial.println(hh);
        int tmpHour = String(hh).toInt();
        
        Serial.println(int(newTime[0]) - 48);
        Serial.println("TMP Hour: ");
        Serial.println(tmpHour);
        char mm[] = {newTime[2], newTime[3],0};
        int tmpMins = String(mm).toInt();

        char ss[] = {newTime[4], newTime[5],0};
        int tmpSecs = String(mm).toInt();
      
        settingNT = false;
        Serial.println("SET NT: ");
        Serial.println(newTime);
//        menuOn = false;
        setTime(tmpHour, tmpMins, tmpSecs, day(), month(), year() );

        ntIndex = 0;
      }
    }


    // Need to put something here that prevents incorect inputs of Time
    //ie no numbers 60 or over and nothing above a 12 for hours


    
    if(customKey == 'A'){
      menuOn = false;
    }else if(customKey =='B'){
      settingNT = true;
      Serial.println("New Time: " );
      Serial.print(newTime);
    }
  }

  if(!menuOn){
      if (timeStatus() == timeSet) {
//    digitalClockDisplay();/
    lcdClockDisplay();
  } else {
    Serial.println("The time has not been set.  Please run the Time");
    Serial.println("TimeRTCSet example, or DS1307RTC SetTime example.");
    Serial.println();
    delay(4000);
    }
   delay(100); 
  }
}

void lcdClockDisplay(){
//  char output[8];/
  Serial.println("test");
  int tmpHour = hour();
  int tmpMins = minute();
  int tmpSecs = second();

  String a = "";
  String b = "";
  String c = "";

  if(tmpHour < 10){
    a = "0" + String(tmpHour);
  }else{
    a = String(tmpHour);
  }
  
  if(tmpMins < 10){
    b = "0" + String(tmpMins);
  }else{
    b = String(tmpMins);
  }
  
  if(tmpSecs < 10){
    c = "0" + String(tmpSecs);
  }else{
    c = String(tmpSecs);
  }
  
  String d = a + ":" + b + ":" + c;

  Serial.println(d);
  lcd.setCursor(1,0);
  lcd.print(d);
}
