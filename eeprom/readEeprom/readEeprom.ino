#include <EEPROM.h>
void setup() {

  Serial.begin(9600);
  for(int i=0; i<=30;i++)
  {
    // if(EEPROM.read(i))
    // Serial.println(i);
    Serial.println(" value =");
    Serial.print(EEPROM.read(i));
    Serial.print(" -- ");
    Serial.print(i);
  }  
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
