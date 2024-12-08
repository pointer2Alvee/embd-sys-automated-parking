//_____________LIBRARIES___________//
#include<LiquidCrystal.h>
#include<Servo.h>
//_____________VARIABLES___________//

// LCD PINS
/* Connect the LCD to pins as shown */
int RS = 7 ;
int EN = 8;
int D4 = 9;
int D5 = 10;
int D6 = 11;
int D7 = 12;

//IR SENSOR PINS
/*CAR PARKING SLOTS IR PINS*/
int SLOT1_IR_PIN = A5;
int SLOT2_IR_PIN = 13;
int SLOT3_IR_PIN = 2;
int SLOT4_IR_PIN = 3;
int SLOT5_IR_PIN = 4;
int SLOT6_IR_PIN = 5;

/*CAR PARKING SLOTS STATS*/
int SLOT1 = 0;
int SLOT2 = 0;
int SLOT3 = 0;
int SLOT4 = 0;
int SLOT5 = 0;
int SLOT6 = 0;

String S1_STATUS;
String S2_STATUS;
String S3_STATUS;
String S4_STATUS;
String S5_STATUS;
String S6_STATUS;

int COUNT_EMPTY_SLOTS;
/*PARKING LOT ENTRY-LEAVE IR PINS*/
int ENTRY_IR_PIN = 6;
int LEAVE_IR_PIN = 0;
/*CAR ENTRY-LEAVE STATUS*/
//0--> EXIT
//1--> ENTRY
//2--> NOTHING
int CAR_ENTRY_LEAVE_STATUS = 2;

//SERVO
int SERVO_PIN = A0;

//ALARM SYSTEM
int RED_LED_PIN = A1;
int GREEN_LED_PIN = A2;
int BUZZER_PIN = A3;


//_____________OBJECTS___________//

//Creating object type - LiquidCrystal
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
Servo myServo;
//_____________FUNCTIONS___________//

//ALARM SYSTEM
void FUNC_ALARM(int ENTRY_LEAVE_STATUS) {
  if (ENTRY_LEAVE_STATUS == 0) {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(500);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
  } else if (ENTRY_LEAVE_STATUS == 1) {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
    delay(500);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
  } else {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
  }
}

//LCD PRINT
void FUCN_INIT() {
  myServo.write(0);
  Serial.print("STARTING CAR PARKING SYSTEM");

  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print("PARKING SYSTEM");


  for (int i = 0; i < 7; i++) {
    Serial.print(".");
    lcd.print(".");
    delay(300);
  }
  lcd.setCursor (4, 1);
  Serial.println(" ");
  delay(100);
  lcd.print("SYSTEM ACTIVATED");
  Serial.println("SYSTEM ACTIVATED");
  delay(1000);

}

void FUNC_LCD_DISPLAY(String S1, String S2, String S3, String S4, String S5, String S6, int EMTSLTS) {

  //LCD Display
  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print("   Have Slot: ");
  lcd.print(EMTSLTS);
  lcd.print("    ");

  lcd.setCursor (0, 1);
  lcd.print("S1: ");
  lcd.print(S1);


  lcd.setCursor (10, 1);
  lcd.print("S2: ");
  lcd.print(S2);


  lcd.setCursor (0, 2);
  lcd.print("S3: ");
  lcd.print(S3);


  lcd.setCursor (10, 2);
  lcd.print("S4: ");
  lcd.print(S4);


  lcd.setCursor (0, 3);
  lcd.print("S5: ");
  lcd.print(S5);


  lcd.setCursor (10, 3);
  lcd.print("S6: ");
  lcd.print(S6);


  //Serial Monitor
  Serial.print(S1);
  Serial.print(" ");
  Serial.print(S2);
  Serial.print(" ");
  Serial.print(S3);
  Serial.print(" ");
  Serial.print(S4);
  Serial.print(" ");
  Serial.print(S5);
  Serial.print(" ");
  Serial.println(S6);
}

//SERVO FUNCTION
void FUCN_BARRIER() {
  int STATE =  FUNC_PARKING_ENTRY_LEAVE();
  //int STATE = 0;
  int pos = 0;
  if (STATE == 0 || STATE == 1) {
     FUNC_ALARM(STATE);
    for (pos = 0; pos <= 105; pos += 1) {
      // in steps of 1 degree
      myServo.write(pos);
      Serial.println(pos);
      delay(15);
    }
    delay(2500);
    for (pos = 105; pos >= 0; pos -= 1) {
      myServo.write(pos);
      Serial.println(pos);
      delay(15);
    }
    //delay(500);

    // delay(100);
    // myServo.write(0);

  } 
  if (STATE == 2 ) {
    myServo.write(0);
      FUNC_ALARM(2);
    // delay(100);
  }
}

//IR FUCNTION

int FUNC_PARKING_ENTRY_LEAVE() {
  if (digitalRead(ENTRY_IR_PIN) == LOW) {
    CAR_ENTRY_LEAVE_STATUS = 1;
    Serial.print(" ");
    Serial.print(digitalRead(ENTRY_IR_PIN));

  } else if (digitalRead(LEAVE_IR_PIN) == LOW) {
    CAR_ENTRY_LEAVE_STATUS = 0;
    Serial.print(" ");
    Serial.print(digitalRead(LEAVE_IR_PIN));
  } else if (digitalRead(ENTRY_IR_PIN) == HIGH && digitalRead(LEAVE_IR_PIN) == HIGH ) {
    CAR_ENTRY_LEAVE_STATUS = 2;
  }
  return CAR_ENTRY_LEAVE_STATUS;
}


void FUNC_READ_IR_INPUTS() {
  SLOT1 = digitalRead(SLOT1_IR_PIN);
  SLOT2 = digitalRead(SLOT2_IR_PIN);
  SLOT3 = digitalRead(SLOT3_IR_PIN);
  SLOT4 = digitalRead(SLOT4_IR_PIN);
  SLOT5 = digitalRead(SLOT5_IR_PIN);
  SLOT6 = digitalRead(SLOT6_IR_PIN);
  Serial.print(SLOT1);
  Serial.print(" ");
  Serial.print(SLOT2);
  Serial.print(" ");
  Serial.print(SLOT3);
  Serial.print(" ");
  Serial.print(SLOT4);
  Serial.print(" ");
  Serial.print(SLOT5);
  Serial.print(" ");
  Serial.println(SLOT6);
}

void FUCN_PARKING_STATUS() {
  FUNC_READ_IR_INPUTS();
  COUNT_EMPTY_SLOTS = 0;
  /* SLOT1 = random(0, 2);
    SLOT2 = random(0, 2);
    SLOT3 = random(0, 2);
    SLOT4 = random(0, 2);
    SLOT5 = random(0, 2);
    SLOT6 = random(0, 2);
  */
  //SLOT - 01
  if (SLOT1 == LOW) {
    S1_STATUS = "FILL";

  } else if (SLOT1 == HIGH) {
    S1_STATUS = "EMPTY";
    COUNT_EMPTY_SLOTS++;
  }
  //SLOT - 02
  if (SLOT2 == LOW) {
    S2_STATUS = "FILL";
  } else if (SLOT2 == HIGH) {
    S2_STATUS = "EMPTY";
    COUNT_EMPTY_SLOTS++;
  }
  //SLOT - 03
  if (SLOT3 == LOW) {
    S3_STATUS = "FILL";
  } else if (SLOT3 == HIGH) {
    S3_STATUS = "EMPTY";
    COUNT_EMPTY_SLOTS++;
  }
  //SLOT - 04
  if (SLOT4 == LOW) {
    S4_STATUS = "FILL";
  } else if (SLOT4 == HIGH) {
    S4_STATUS = "EMPTY";
    COUNT_EMPTY_SLOTS++;
  }
  //SLOT - 05
  if (SLOT5 == LOW) {
    S5_STATUS = "FILL";
  } else if (SLOT5 == HIGH) {
    S5_STATUS = "EMPTY";
    COUNT_EMPTY_SLOTS++;
  }
  //SLOT - 06
  if (SLOT6 == LOW) {
    S6_STATUS = "FILL";
  } else if (SLOT6 == HIGH) {
    S6_STATUS = "EMPTY";
    COUNT_EMPTY_SLOTS++;
  }

  FUNC_LCD_DISPLAY(S1_STATUS, S2_STATUS, S3_STATUS, S4_STATUS, S5_STATUS, S6_STATUS, COUNT_EMPTY_SLOTS);

}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); // give 9600 here and 2400  on Serial Monitor
  //INIT FUNCTION
  FUCN_INIT();

  //IR PINS
  pinMode(SLOT1_IR_PIN, INPUT);
  pinMode(SLOT2_IR_PIN, INPUT);
  pinMode(SLOT3_IR_PIN, INPUT);
  pinMode(SLOT4_IR_PIN, INPUT);
  pinMode(SLOT5_IR_PIN, INPUT);
  pinMode(SLOT6_IR_PIN, INPUT);

  pinMode(ENTRY_IR_PIN, INPUT);
  pinMode(LEAVE_IR_PIN, INPUT);

  //ALARM PINS
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  //LCD DISPLAY
  lcd.begin(20, 4);

  //SERVO
  myServo.attach(SERVO_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  FUCN_BARRIER();
  FUCN_PARKING_STATUS();
  //FUCN_BARRIER();
  // lcd.print("HELLO ");

  //delay(10000);
}
