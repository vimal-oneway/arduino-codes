/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <EEPROM.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;

#define RESULT 5
#define ENROLL 6
#define VOTE 7

#define CAN1 8
#define CAN2 9
#define CAN3 10

#define EEPROM_VOTE_CAN_1 0
#define EEPROM_VOTE_CAN_2 1
#define EEPROM_VOTE_CAN_3 2
#define EEPROM_ID 3
#define EEPROM_INDEX 4
 
int vote1, vote2, vote3, eepromIndex;
uint8_t id = 1;

void setup() {
  //Fingerprint sensor module setup
  Serial.begin(9600);
  // set the data rate for the sensor serial port
  finger.begin(57600);
  lcd.init();
  lcd.backlight();

  pinMode(CAN1, INPUT);
  pinMode(CAN2, INPUT);
  pinMode(CAN3, INPUT);
  pinMode(VOTE, INPUT);
  pinMode(ENROLL, INPUT);
  pinMode(RESULT, INPUT);

  digitalWrite(VOTE, HIGH);
  digitalWrite(ENROLL, HIGH);
  digitalWrite(RESULT, HIGH);
  digitalWrite(CAN1, HIGH);
  digitalWrite(CAN2, HIGH);
  digitalWrite(CAN3, HIGH);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    lcd.clear();
    lcd.print("module Found");
    lcd.setCursor(0, 1);
    lcd.print("Ready to vote");
  } else {
    Serial.println("Did not find fingerprint sensor :(");

    lcd.clear();
    lcd.print("module not Found");
    lcd.setCursor(0, 1);
    lcd.print("Check Connections");

    while (1) { delay(1); }
  }

  vote1 = EEPROM.read(EEPROM_VOTE_CAN_1);
  vote2 = EEPROM.read(EEPROM_VOTE_CAN_2);
  vote3 = EEPROM.read(EEPROM_VOTE_CAN_3);
  eepromIndex = EEPROM.read(EEPROM_INDEX);


  lcd.clear();
  lcd.print("Electronic");
  lcd.setCursor(0, 1);
  lcd.print("Voting Machine");
  delay(1000);

  if (!EEPROM.read(3)) {
    EEPROM.write(EEPROM_ID, 1);
  }

  if (!EEPROM.read(4)) {
    EEPROM.write(EEPROM_INDEX, 4);
  }

  finger.getParameters();
}

uint8_t readID(void) {
  return EEPROM.read(EEPROM_ID);
}

void incrementID(uint8_t id) {
  EEPROM.write(EEPROM_ID, ++id);
}

int getIndex() {
  return EEPROM.read(EEPROM_INDEX);
}

int getIdIndex(int id) {
  for (int i = 5; i <= EEPROM.length(); i++) {
    Serial.print(EEPROM.read(i));
    Serial.print(" -- ");
    Serial.print(i);
    Serial.println(" ");
    int val = EEPROM.read(i);
    if (val == id) {
      return i;
      break;
    }
  }
  return -1;
}

void incrementIndex() {
  int val = EEPROM.read(EEPROM_INDEX);
  EEPROM.write(EEPROM_INDEX, val + 2);
}

void loop() {
  lcd.clear();
  lcd.print("Press the key to");
  lcd.setCursor(0, 1);
  lcd.print("start the system");

  if (digitalRead(VOTE) == 0) {
    vote();  // 7 pin
  } else if (digitalRead(RESULT) == 0) {
    result();  // 5 pin
  } else if (digitalRead(ENROLL) == 0) {
    enroll();  // 6 pin
  }
  delay(500);
}

void vote() {
  lcd.clear();
  lcd.print("Place your finger");
  fingerprintID = getFingerprintID();

  if(fingerprintID==9){
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Fingerprint not found");
      return;
  }

  Serial.println("Fingerprint id : " + fingerprintID);

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Your voter id ");
  lcd.print(fingerprintID);

  delay(600);


  int idIndex = getIdIndex(fingerprintID);
  Serial.println("index id " + idIndex);

  if (idIndex == -1) {
    lcd.clear();
    lcd.print("ID not found");
    return;
  }

  if (idIndex != -1 && EEPROM.read(idIndex) == fingerprintID) {
    if (EEPROM.read(idIndex + 1) != 1) {
      setVote(idIndex);
    } else {
      lcd.clear();
      lcd.print("Already voted");
    }
  }

  if (fingerprintID == -1) {
    lcd.clear();
    lcd.print("ID not found");
    lcd.setCursor(0, 1);
    lcd.print("Are you enrolled");
    delay(2000);
  }
}

void setVote(int id) {
  int c1 = 0;
  int c2 = 0;
  int c3 = 0;

  Serial.println(id);
  Serial.println("your index value");

  while (1) {
    lcd.clear();
    lcd.print("Select canidate");
    c1 = digitalRead(CAN1);
    c2 = digitalRead(CAN2);
    c3 = digitalRead(CAN3);
    if (c1 == 0) {
      int c1_vote = EEPROM.read(0);
      vote1 = c1_vote + 1;
      EEPROM.write(0, vote1);
      EEPROM.write(id + 1, 1);
      lcd.clear();
      lcd.print("Voted to CAN 1");
      break;
    } else if (c2 == 0) {
      int c2_vote = EEPROM.read(1);
      vote2 = c2_vote + 1;
      EEPROM.write(1, vote2);
      EEPROM.write(id + 1, 1);
      lcd.clear();
      lcd.print("Voted to CAN 2");
      break;
    } else if (c3 == 0) {
      int c3_vote = EEPROM.read(2);
      vote3 = c3_vote + 1;
      EEPROM.write(2, vote3);
      EEPROM.write(id + 1, 1);
      lcd.clear();
      lcd.print("Voted to CAN 3");
      break;
    }
  }
}

void result() {
  vote1 = EEPROM.read(EEPROM_VOTE_CAN_1);
  vote2 = EEPROM.read(EEPROM_VOTE_CAN_2);
  vote3 = EEPROM.read(EEPROM_VOTE_CAN_3);
  lcd.clear();

  String line = "result c1-";
  line.concat(vote1);
  line.concat(" c2-");
  line.concat(vote2);
  lcd.print(line);
  line = "c3-";
  line.concat(vote3);
  lcd.setCursor(0, 1);
  lcd.print(line);
  delay(3000);

  line = "";
  if (vote1 > vote2 && vote1 > vote3)
    line.concat("c1 win");
  else if (vote2 > vote1 && vote2 > vote3)
    line.concat("c2 win");
  else if (vote3 > vote1 && vote3 > vote2)
    line.concat("c3 win");
  else
    line="Draw";
  lcd.clear();
  lcd.print(line);
  delay(3000);
}

void enroll() {
  lcd.clear();
  lcd.print("Fingerprint enroll");
  lcd.setCursor(0, 1);
  lcd.print("place your finger");

  id = readID();

  if (id == 0) {  // ID #0 not allowed, try again!
    lcd.clear();
    lcd.print("Unable to enroll now");
    return;
  }

  // Serial.print("Enrolling ID #");
  // Serial.println(id);

  while (!getFingerprintEnroll())
    ;

  lcd.clear();
  Serial.print("loop breaked");

  int incrementVal = getIndex();
  EEPROM.write(incrementVal + 1, id);
  EEPROM.write(incrementVal + 2, 0);
  incrementID(id);
  incrementIndex();

  lcd.clear();
  lcd.print("your id: ");
  lcd.print(id);
  lcd.setCursor(0, 1);
  lcd.print("enroll success");
  lcd.clear();
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  lcd.clear();
  lcd.print("Waiting for valid finger to enroll as #");
  lcd.println(id);
  Serial.print("Waiting for valid finger to enroll as #");
  Serial.println(id);

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.clear();
        lcd.println("Image taken");
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        // lcd.clear();
        // lcd.println(".");
        // Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.clear();

        lcd.println("Communication error");
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.println("Imaging error");
        Serial.println("Imaging error");
        break;
      default:
        lcd.clear();
        lcd.println("Unknown error");
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.println("Image converted");
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.println("Image too messy");
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.println("Communication error");
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.println("Could not find fingerprint features");
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.println("Could not find fingerprint features");
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      lcd.clear();
      lcd.println("Unknown error");
      Serial.println("Unknown error");
      return p;
  }

  lcd.clear();
  lcd.println("Remove finger");
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  lcd.clear();
  lcd.print("ID ");
  Serial.print("ID ");
  lcd.println(id);
  Serial.println(id);
  p = -1;
  lcd.clear();
  lcd.println("Place same finger again");
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.clear();
        lcd.println("Image taken");
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        // lcd.clear();
        // lcd.print(".");
        // Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.clear();
        lcd.println("Communication error");
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.println("Imaging error");
        Serial.println("Imaging error");
        break;
      default:
        lcd.clear();
        lcd.println("Unknown error");
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.println("Image converted");
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.println("Image too messy");
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.println("Communication error");
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.println("Could not find fingerprint features");
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.println("Could not find fingerprint features");
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      lcd.clear();
      lcd.println("Unknown error");
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  lcd.clear();

  lcd.print("Creating model for #");
  Serial.print("Creating model for #");
  lcd.println(id);
  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.println("Prints matched!");
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.println("Communication error");
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    lcd.println("Fingerprints did not match");
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    lcd.clear();
    lcd.println("Unknown error");
    Serial.println("Unknown error");
    return p;
  }

  lcd.clear();
  lcd.print("ID ");
  lcd.println(id);
  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.println("Stored!");
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.println("Communication error");
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    lcd.println("Could not store in that location");
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();
    lcd.println("Error writing to flash");
    Serial.println("Error writing to flash");
    return p;
  } else {
    lcd.clear();
    lcd.println("Unknown error");
    Serial.println("Unknown error");
    return p;
  }

  return true;
}



uint8_t getFingerprintID() {
  Serial.println("getFingerprintID openning ");
  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println("No finger detected");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);

  return finger.fingerID;
}
