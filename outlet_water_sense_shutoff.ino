#include <SevenSegmentDisplay.h>

int waterPower = 20;          

int waterSensePin = 26;
int potSensePin = 27;
int showPotPin = 21;
int showWaterLevelPin = 22;
int powerOutPin = 28;
int powerIndicatorPin = 2;
int waterDetectedTime = false;

SevenSegmentDisplay lcd(17,16,14,13,12,18,19,15, false);

void setup() {
  // put your setup code here, to run once:
  pinMode(waterPower, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(powerOutPin, OUTPUT);
  pinMode(powerIndicatorPin, OUTPUT);

  pinMode(waterSensePin, INPUT);
  pinMode(potSensePin, INPUT);
  
  pinMode(showPotPin, INPUT);
  pinMode(showWaterLevelPin, INPUT);

  digitalWrite(powerOutPin, HIGH);
  digitalWrite(waterPower, LOW);
}

void loopNum(int num, int delayMS){
  String str = String(num);
  for(int i=0;i<str.length();i++){
    lcd.displayCharacter(str.charAt(i));
    delay(delayMS);
    lcd.displayCharacter(' ');
    delay(50);
  }
}

int readWater(){
  digitalWrite(waterPower, HIGH);
  delay(10);
  int waterLevel = analogRead(waterSensePin);
  delay(10);
  digitalWrite(waterPower, LOW);
  delay(10);
  return waterLevel;
}

void loop() {  
  bool shouldShowPot = digitalRead(showPotPin);
  bool shouldShowWater = digitalRead(showWaterLevelPin);

  if(waterDetectedTime > 0){
    digitalWrite(powerOutPin, LOW);
    digitalWrite(powerIndicatorPin, HIGH);
    lcd.displayCharacter('F');
    delay(500);
    loopNum((millis()/1000 - waterDetectedTime)/60, 250);
    delay(500);
    lcd.displayCharacter('m');
    delay(1000); 
    
    // if both buttons are pressed - reset the water detected
    if(shouldShowPot && shouldShowWater){
      waterDetectedTime = 0;
      digitalWrite(powerIndicatorPin, LOW);
      digitalWrite(powerOutPin, HIGH);
    } else {
      return;
    }
  }

  int potValue = analogRead(potSensePin);
  int waterLevel = readWater();

  if(waterLevel > potValue){
    waterDetectedTime = millis() / 1000;
    return;
  }

  if(shouldShowPot){
    lcd.displayCharacter('P');
    delay(200);
    loopNum(potValue, 500);
    delay(1000);
  }
  if(shouldShowWater) {
    lcd.displayCharacter('W');
    delay(200);
    loopNum(waterLevel, 500);
    delay(1000);
  };
  
  delay(1000);
}
