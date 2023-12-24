#include <EEPROM.h>

void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:
  for(int i =0; i<EEPROM.length(); i++){
    Serial.print("deleting");
    Serial.print(EEPROM.read(i));
    EEPROM.write(i,0);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
