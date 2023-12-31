/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <EEPROM.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t fingerprintID = -1;

#define RESULT 14
#define ENROLL 15
#define VOTE 16
#define DELETEFUNC 17

#define CAN1 13
#define CAN2 12
#define CAN3 11
#define CAN4 10
#define CAN5 9
#define CAN6 8
#define CAN7 7
#define CAN8 6

#define CAN1INDEX 0
#define CAN2INDEX 1
#define CAN3INDEX 2
#define CAN4INDEX 3
#define CAN5INDEX 4
#define CAN6INDEX 5
#define CAN7INDEX 6
#define CAN8INDEX 7


#define LEDVOTE 4
#define LEDRESULT 5

#define EEPROM_ID 8
#define EEPROM_INDEX 9
int vote1;
int vote2;
int vote3;
int vote4;
int vote5;
int vote6;
int vote7;
int vote8;
uint8_t id = 1;
// int votes[7];

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
  pinMode(CAN4, INPUT);
  pinMode(CAN5, INPUT);
  pinMode(CAN6, INPUT);
  pinMode(CAN7, INPUT);
  pinMode(CAN8, INPUT);

  pinMode(VOTE, INPUT);
  pinMode(ENROLL, INPUT);
  pinMode(RESULT, INPUT);

  pinMode(DELETEFUNC, INPUT);
  pinMode(LEDRESULT, OUTPUT);
  pinMode(LEDVOTE, OUTPUT);

  digitalWrite(VOTE, HIGH);
  digitalWrite(ENROLL, HIGH);
  digitalWrite(RESULT, HIGH);
  digitalWrite(CAN1, HIGH);
  digitalWrite(CAN2, HIGH);
  digitalWrite(CAN3, HIGH);
  digitalWrite(CAN4, HIGH);
  digitalWrite(CAN5, HIGH);
  digitalWrite(CAN6, HIGH);
  digitalWrite(CAN7, HIGH);
  digitalWrite(CAN8, HIGH);
  digitalWrite(DELETEFUNC, HIGH);
  digitalWrite(LEDRESULT, LOW);
  digitalWrite(LEDVOTE, LOW);

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

  lcd.clear();
  lcd.print("Electronic");
  lcd.setCursor(0, 1);
  lcd.print("Voting Machine");
  delay(1000);

  if (!EEPROM.read(EEPROM_ID)) {
    EEPROM.write(EEPROM_ID, 1);
  }

  if (!EEPROM.read(EEPROM_INDEX)) {
    EEPROM.write(EEPROM_INDEX, EEPROM_INDEX);
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
  for (int i = EEPROM_INDEX + 1; i <= EEPROM.length(); i++) {
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
  } else if (digitalRead(DELETEFUNC) == 0) {
    deleteAllMemory();
  }
  delay(500);
}

void vote() {
  lcd.clear();
  lcd.print("Place your");
  lcd.setCursor(0, 1);
  lcd.print("Finger to vote");
  delay(2000);

  fingerprintID = getFingerprintID();

  Serial.print(fingerprintID + " was found");


  if (fingerprintID == 9) {
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
  int c4 = 0;
  int c5 = 0;
  int c6 = 0;
  int c7 = 0;
  int c8 = 0;

  Serial.println(id);
  Serial.println("your index value");
  lcd.clear();
  lcd.print("Select canidate");
  while (1) {
    c1 = digitalRead(CAN1);
    c2 = digitalRead(CAN2);
    c3 = digitalRead(CAN3);
    c4 = digitalRead(CAN4);
    c5 = digitalRead(CAN5);
    c6 = digitalRead(CAN6);
    c7 = digitalRead(CAN7);
    c8 = digitalRead(CAN8);
    if (c1 == 0) {
      digitalWrite(LEDVOTE, HIGH);
      EEPROM.write(CAN1INDEX, EEPROM.read(CAN1INDEX) + 1);
      EEPROM.write(id + 1, 1);
      lcd.clear();
      lcd.print("Voted to CAN 1");
      digitalWrite(LEDVOTE, LOW);
      delay(2000);
      break;
    } else if (c2 == 0) {
      digitalWrite(LEDVOTE, HIGH);
      EEPROM.write(CAN2INDEX, EEPROM.read(CAN2INDEX) + 1);
      EEPROM.write(id + 1, 1);
      lcd.clear();
      lcd.print("Voted to CAN 2");
      digitalWrite(LEDVOTE, LOW);
      delay(2000);
      break;
    } else if (c3 == 0) {
      digitalWrite(LEDVOTE, HIGH);
      EEPROM.write(CAN3INDEX, EEPROM.read(CAN3INDEX) + 1);
      EEPROM.write(id + 1, 1);
      lcd.clear();
      lcd.print("Voted to CAN 3");
      digitalWrite(LEDVOTE, LOW);
      delay(2000);
      break;
    } else if (c4 == 0) {
      digitalWrite(LEDVOTE, HIGH);
      EEPROM.write(CAN4INDEX, EEPROM.read(CAN4INDEX) + 1);
      EEPROM.write(id + 1, 1);
      lcd.clear();
      lcd.print("Voted to CAN 4");
      digitalWrite(LEDVOTE, LOW);
      delay(2000);
      break;
    } else if (c5 == 0) {
      digitalWrite(LEDVOTE, HIGH);
      EEPROM.write(CAN5INDEX, EEPROM.read(CAN5INDEX) + 1);
      EEPROM.write(id + 1, 1);
      lcd.clear();
      lcd.print("Voted to CAN 5");
      digitalWrite(LEDVOTE, LOW);
      delay(2000);
      break;
    } else if (c6 == 0) {
      digitalWrite(LEDVOTE, HIGH);
      EEPROM.write(CAN6INDEX, EEPROM.read(CAN6INDEX) + 1);
      EEPROM.write(id + 1, 1);
      lcd.clear();
      lcd.print("Voted to CAN 6");
      digitalWrite(LEDVOTE, LOW);
      delay(2000);
      break;
    } else if (c7 == 0) {
      digitalWrite(LEDVOTE, HIGH);
      EEPROM.write(CAN7INDEX, EEPROM.read(CAN7INDEX) + 1);
      EEPROM.write(id + 1, 1);
      lcd.clear();
      lcd.print("Voted to CAN 7");
      digitalWrite(LEDVOTE, LOW);
      delay(2000);
      break;
    } else if (c8 == 0) {
      digitalWrite(LEDVOTE, HIGH);
      EEPROM.write(CAN8INDEX, EEPROM.read(CAN8INDEX) + 1);
      EEPROM.write(id + 1, 1);
      lcd.clear();
      lcd.print("Voted to CAN 8");
      digitalWrite(LEDVOTE, LOW);
      delay(2000);
      break;
    }
  }
}


// String getResult(int vote1, int vote2, int vote8) {

//   if (vote1 == 0 && vote2 == 0 && vote3 == 0)
//     return "No vote populated";

//   if (vote1 > vote2 && vote1 > vote3)
//     return "c1 win";
//   else if (vote2 > vote1 && vote2 > vote3)
//     return "c2 win";
//   else if (vote3 > vote1 && vote3 > vote2)
//     return "c3 win";
//   else if (vote1 == vote2 || vote2 == vote3 || vote3 == vote1)
//     return "vote Tied";
// }



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
  delay(2000);

  lcd.clear();
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  lcd.clear();

  lcd.print("Waiting....");
  lcd.setCursor(0, 1);
  lcd.print("for Enroll id:");
  lcd.print(id);

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
        delay(2000);
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.println("Imaging error");
        delay(2000);

        Serial.println("Imaging error");
        break;
      default:
        lcd.clear();
        lcd.println("Unknown error");
        delay(2000);
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
      delay(2000);
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.println("Image too messy");
      Serial.println("Image too messy");
      delay(2000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.println("Communication error");
      Serial.println("Communication error");
      delay(2000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.println("Could not find fingerprint features");
      Serial.println("Could not find fingerprint features");
      delay(2000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.println("Could not find fingerprint features");
      Serial.println("Could not find fingerprint features");
      delay(2000);
      return p;
    default:
      lcd.clear();
      lcd.println("Unknown error");
      Serial.println("Unknown error");
      delay(2000);
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
  lcd.print("Place same");
  lcd.setCursor(0, 1);
  lcd.print("finger again");
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.clear();
        lcd.println("Image taken");
        Serial.println("Image taken");
        delay(2000);
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
        delay(2000);
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.println("Imaging error");
        Serial.println("Imaging error");
        delay(2000);

        break;
      default:
        lcd.clear();
        lcd.println("Unknown error");
        Serial.println("Unknown error");
        delay(2000);
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.println("Image converted");
      delay(2000);
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.println("Image too messy");
      delay(2000);
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.println("Communication error");
      delay(2000);
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.println("Could not find fingerprint features");
      delay(2000);
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.println("Could not find fingerprint features");
      delay(2000);
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      lcd.clear();
      lcd.println("Unknown error");
      Serial.println("Unknown error");
      delay(2000);
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
    delay(2000);
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.println("Communication error");
    delay(2000);
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    lcd.println("Fingerprints did not match");
    delay(2000);
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    lcd.clear();
    lcd.println("Unknown error");
    delay(2000);
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
    delay(2000);
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.println("Communication error");
    delay(2000);
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    lcd.println("Could not store in that location");
    Serial.println("Could not store in that location");
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();
    lcd.println("Error writing to flash");
    delay(2000);
    Serial.println("Error writing to flash");
    return p;
  } else {
    lcd.clear();
    lcd.println("Unknown error");
    delay(2000);
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

void deleteAllMemory() {

  lcd.clear();
  lcd.print("Deleting...");

  for (int i = 0; i < EEPROM.length(); i++) {
    Serial.print("deleting");
    Serial.print(EEPROM.read(i));
    EEPROM.write(i, 0);
  }

  Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");

  // delete all
  int i = 1;
  while (i <= 255) {
    deleteFingerprint(i);
    i++;
  }

  // Delete particular id
  // deleteFingerprint(9);
  lcd.clear();
  lcd.print("Deleted");
}


uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
  } else {
    Serial.print("Unknown error: 0x");
    Serial.println(p, HEX);
  }

  return p;
}


// vote1 = EEPROM.read(EEPROM_VOTE_CAN_1);
// vote2 = EEPROM.read(EEPROM_VOTE_CAN_2);
// vote3 = EEPROM.read(EEPROM_VOTE_CAN_3);
// lcd.clear();



// digitalWrite(LEDRESULT, HIGH);

// // get value form EEPROM
// Serial.println(" get eeprom votes ");

// // Calucaling who is winner
// int maxVote = -1, winner = -1, total = 0, prevMaxVote = -1;
// // int i = 0;
// for (int i = 0; i <= 7; i++) {
//   votes[i] = EEPROM.read(i);
//   // Serial.println("");
//   // Serial.print(i);
//   // Serial.print(" -- ");
//   // Serial.print(votes[i]);


//   // Serial.println("");
//   // Serial.print("Entering ---");

//   // Serial.println("");
//   // Serial.print("========");
//   // Serial.print(i);
//   // Serial.print(" == ");
//   // Serial.print(votes[i]);
//   // Serial.print("========");
//   total += votes[i];
//   if ( votes[i] > maxVote && votes[i] != 0 && i < 8) {
//     maxVote = votes[i];
//     winner = i;
//     // Serial.println("");
//     // Serial.print("========");
//     // Serial.print("Setting votes");
//     // Serial.print(votes[i]);
//     // Serial.print("========");
//     // Serial.print(i);
//   }
//     // prevMaxVote = maxVote;
// }
// for (int i = 0; i <= 7; i++) {
//   if (votes[i] == maxVote && i != winner) {
//     maxVote = -1;
//     winner = -1;
//   }
// }


// Serial.println("");
// Serial.print("\n Winner is ");
// Serial.print(winner);
void result() {
vote1 = EEPROM.read(CAN1INDEX);
  vote2 = EEPROM.read(CAN2INDEX);
  vote3 = EEPROM.read(CAN3INDEX);
  lcd.clear();

  digitalWrite(LEDRESULT, HIGH);

  lcd.print("result c1-");
  lcd.print(vote1);
  lcd.print(" c2-");
  lcd.print(vote2);
  lcd.setCursor(0, 1);

  lcd.print("c3-");
  lcd.print(vote3);
  delay(3000);

  lcd.clear();

  // int totalVote = vote1 + vote2 + vote3;

  if (vote1 == 0 && vote2 == 0 && vote3 == 0) {
    // Serial.println("in else");
    lcd.print("No vote populated");

  } else {
    //  Serial.println("in if");

    if (vote1 > vote2 && vote1 > vote3) {
      lcd.print("c1 win");
      // Serial.println("in c1");
    } else if (vote2 > vote1 && vote2 > vote3) {
      // Serial.println("in c2");
      lcd.print("c2 win");
    } else if (vote3 > vote1 && vote3 > vote2) {
      // Serial.println("in c3");
      lcd.print("c3 win");
    } else {
      // Serial.println("in tied");
      lcd.print("vote tied");
    }
  }

  delay(3000);
  digitalWrite(LEDRESULT, LOW);


  // vote1 = EEPROM.read(CAN1INDEX);
  // vote2 = EEPROM.read(CAN2INDEX);
  // vote3 = EEPROM.read(CAN3INDEX);
  // vote4 = EEPROM.read(CAN4INDEX);
  // vote5 = EEPROM.read(CAN5INDEX);
  // vote6 = EEPROM.read(CAN6INDEX);
  // vote7 = EEPROM.read(CAN7INDEX);
  // vote8 = EEPROM.read(CAN8INDEX);
  
  

  // digitalWrite(LEDRESULT, HIGH);

  // // lcd.clear();
  // // lcd.print("result c1-");
  // // lcd.print(vote1);
  // // lcd.print(" c2-");
  // // lcd.print(vote2);
  // // lcd.clear();


  // delay(3000);
  // lcd.setCursor(0, 1);
  // lcd.print("c3-");
  // lcd.print(vote3);
  // lcd.print("c4-");
  // lcd.print(vote4);
  // lcd.clear();
  // lcd.print("result c5-");
  // lcd.print(vote5);
  // lcd.print(" c6-");
  // lcd.print(vote6);
  // lcd.setCursor(0, 1);
  // lcd.print("c7-");
  // lcd.print(vote7);
  // lcd.print("c8-");
  // lcd.print(vote8);
  // delay(3000);


  // lcd.clear();

  // int totalVote = vote1 + vote2 + vote3;

  // if (vote1 == 0 && vote2 == 0 && vote3 == 0 && vote4 == 0 && vote5 == 0 && vote6 == 0 && vote7 == 0 && vote8 == 0) {
  //   // Serial.println("in else");
  //   lcd.print("No vote populated");

  // } else {
  //   //  Serial.println("in if");

    // if (vote1 > vote2 && vote1 > vote3 && vote1 > vote4 && vote1 > vote5 && vote1 > vote6 && vote1 > vote7 && vote1 > vote8) {
    //   lcd.print("c1 win");
    //   // Serial.println("in c1");
    // } else if (vote2 > vote1 && vote2 > vote3 && vote2 > vote4 && vote2 > vote5 && vote2 > vote6 && vote2 > vote7 && vote2 > vote8) {
    //   // Serial.println("in c2");
    //   lcd.print("c2 win");
    // } else if (vote3 > vote1 && vote3 > vote2 && vote3 > vote4 && vote3 > vote5 && vote3 > vote6 && vote3 > vote7 && vote3 > vote8) {
    //   // Serial.println("in c3");
    //   lcd.print("c3 win");
    // }
    // else if ((vote4 > vote1 && vote4 > vote2 && vote4 > vote3 && vote4 > vote5 && vote4 > vote6 && vote4 > vote7 && vote4 > vote8)) {
    //   lcd.print("c4 win");
    // } else if ((vote5 > vote1 && vote5 > vote2 && vote5 > vote3 && vote5 > vote4 && vote5 > vote6 && vote5 > vote7 && vote5 > vote8)) {
    //   lcd.print("c5 win");
    // } else if ((vote6 > vote1 && vote6 > vote2 && vote6 > vote3 && vote6 > vote4 && vote6 > vote5 && vote6 > vote7 && vote6 > vote8)) {
    //   lcd.print("c6 win");
    // } else if ((vote7 > vote1 && vote7 > vote2 && vote7 > vote3 && vote7 > vote4 && vote7 > vote5 && vote7 > vote6 && vote7 > vote8)) {
    //   lcd.print("c7 win");
    // } else if ((vote8 > vote1 && vote8 > vote2 && vote8 > vote3 && vote8 > vote4 && vote8 > vote5 && vote8 > vote6 && vote8 > vote7)) {
    //   lcd.print("c8 win");
    // } else {
    //   // Serial.println("in tied");
    //   lcd.print("vote tied");
    // }
  // }

  // delay(3000);
  // digitalWrite(LEDRESULT, LOW);
}
// //   while (i<8) {
// //     if(i==8)
// // {
// //   break;
// // }


// // Serial.println("");
// // Serial.print("Entering ---");


// // Serial.println("");

// // Serial.print("========");
// // Serial.print(i);
// // Serial.print(" == ");
// // Serial.print(votes[i]);
// // Serial.print("========");

// //   if (votes[i] >  maxVote && votes[i] !=0 && i<8) {
// //     maxVote = votes[i];
// //     winner = i;
// // Serial.println("");
// // Serial.print("========");
// // Serial.print("Setting votes");
// // Serial.print(votes[i]);
// // Serial.print("========");
// // Serial.print(i);

// //   }

// //   i++;

// // }

// // for (i; i < EEPROM_ID; i++) {

// // }

// // Serial.println("");

// // Serial.print("\n Winner is ");
// // Serial.print(winner);
// // Serial.print("\n score is ");
// // Serial.print(maxVote);


// // lcd.print("result c1-");
// // lcd.print(vote1);
// // lcd.print(" c2-");
// // lcd.print(vote2);
// // lcd.setCursor(0, 1);

// // lcd.print("c3-");
// // lcd.print(vote3);
// // delay(3000);

// // lcd.clear();


// // if (vote1 == 0 && vote2 == 0 && vote3 == 0) {
// //   lcd.print("No vote populated");
// // } else {
// //   if (vote1 > vote2 && vote1 > vote3) {
// //     lcd.print("c1 win");
// //   } else if (vote2 > vote1 && vote2 > vote3) {
// //     lcd.print("c2 win");
// //   } else if (vote3 > vote1 && vote3 > vote2) {
// //     lcd.print("c3 win");
// //   } else {
// //     lcd.print("vote tied");
// //   }
// // }

// delay(3000);
// digitalWrite(LEDRESULT, LOW);
// }
