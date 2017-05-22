#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 7,6,5,4);
SoftwareSerial BTSerial(2,3);

int count=1;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  lcd.begin(20, 4);
 // lcd.print("I love Jessica!");
}

void loop() {
 // lcd.display();
  if (BTSerial.available()){
    char inchar = (char)BTSerial.read();
    if(inchar=='*'){
      lcd.clear();
    }
    else if(inchar=='/'){
      //Serial.println();
      lcd.setCursor(0,count);
      count++;
      if(count==4){
        count=0;
      
      }
    }
    else{
    lcd.write(inchar);
    //Serial.write(inchar);
    }
  }

}

