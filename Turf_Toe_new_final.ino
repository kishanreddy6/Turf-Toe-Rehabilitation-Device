#include <Servo.h>
#include <SoftwareSerial.h>
#include <ParallaxLCD.h>

ParallaxLCD lcd(7, 2, 16); // desired pin, rows, cols

Servo pitchservo;
Servo yawservo;

int screen = 1;
int pitch_reps = 2;
int yaw_reps = 2;
int pitch_positionDesired;
int pitch_home = 130;
int yaw_positionDesired;
int yaw_home = 120;
int pitch_counter = 0;
int yaw_counter = 0;
int pitch_neutralpos;
int yaw_neutralpos;
int flag = 1;
int pitch_check = 0;
int yaw_check = 0;
int passive_count;
int passive_rep = 2;
int active;
int passive;

const int button1 = 2;     // the number of the pushbutton pin
const int button2 = 3;     // the number of the pushbutton pin


int button1_state = 0;     // variable for reading the pushbutton status
int button2_state = 0;     // variable for reading the pushbutton status


int Lswitch = 4;           // the number of the pushbutton pin

void setup() {
  //initializing LCD
  lcd.setup();
  delay(1000);
  lcd.backLightOn();
  delay(500);
  lcd.off();

  // initialize the pushbutton pin as an input:
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pitchservo.attach(9);
  pitchservo.write(pitch_home);
  yawservo.attach(10);
  yawservo.write(yaw_home);
  pinMode(Lswitch, INPUT);
}


void loop() {

  //flags to print once on the LCD

  if (screen == 1) {
    lcd.on();
    lcd.empty();
    lcd.print("Btn1 for phase1\0");
    lcd.pos(1, 0);
    lcd.print("Btn2 for phase2\0");
    screen = 2;
  }

  // read the state of the pushbutton value:

  button1_state = digitalRead(button1);
  button2_state = digitalRead(button2);


  // check if the pushbutton are pressed. and go to respective Phases:

  if (button1_state == HIGH && button2_state == LOW) {
    lcd.empty();
    lcd.print("Entering Phase1 in 2s");
    delay(2000);
    lcd.empty();
    active = 1;
    Active();
  }

  else if (button1_state == LOW && button2_state == HIGH) {
    lcd.empty();
    lcd.print("Entering Phase2 in 2s");
    delay(2000);
    lcd.empty();
    passive = 1;
    Passive();
  }
  else {
    button1_state = digitalRead(button1);
    button2_state = digitalRead(button2);
    flag = 1;
    passive_count = 0;
    pitch_check = 0;
    yaw_check = 0;
  }
}

//------------------------------------Active Rehab (Phase-1)----------------------------------------------

void Active() {

  while (flag == 1) {
    // read the state of the pushbutton value:
    button1_state = 0;
    button2_state = 0;
    // check if the pushbuttons are pressed and go to respective motions:
    button1_state = digitalRead(button1);
    button2_state = digitalRead(button2);

    //--------------------------Pitch-------------------------

    if (button1_state == HIGH && button2_state == LOW ) {
      lcd.empty();
      lcd.print("Pitch Motion in 2s");
      delay(2000);
      pitchservo.attach(9);
      lcd.empty();
      pitchservo.write(pitch_home);
      yawservo.write(yaw_home);

      while (pitch_counter <= pitch_reps) {
        for (pitch_positionDesired = 180; pitch_positionDesired >= 90; pitch_positionDesired -= 1) {    // goes from 180 degrees to 90 degrees mapped to 70 to -30deg in real case
          pitchservo.write(pitch_positionDesired);                                                      // tell servo to go to position in variable 'pitch_positionDesired'
          delay(25);                                                                                    // waits 25 ms for the servo to reach the position
        }

        // motor Pitch movement
        for (pitch_positionDesired = 90; pitch_positionDesired <= 180; pitch_positionDesired += 1) {     // goes from 90 degrees to 180 degrees in steps of 1 degree
          pitchservo.write(pitch_positionDesired);                                                       // tell servo to go to position in variable 'pitch_positionDesired'
          delay(25);                                                                                     // waits 25 ms for the servo to reach the position
        }

        pitch_counter++;
      }
      pitchservo.write(pitch_home);
      yawservo.write(yaw_home);
      pitch_check = 1;
      active = 1;
      screen = 1;
    }


    //--------------------------Yaw-------------------------

    else if (button1_state == LOW && button2_state == HIGH ) {
      lcd.empty();
      lcd.print("Yaw Motion in 2s");
      delay(2000);
      yawservo.write(yaw_home);
      pitchservo.write(pitch_home);
      lcd.empty();

      while (yaw_counter <= yaw_reps) {
        // motor Yaw movement
        yawservo.write(yaw_home);
        pitchservo.write(pitch_home);
        for (yaw_positionDesired = 110; yaw_positionDesired <= 140; yaw_positionDesired += 1) {   // goes from 110 degrees to 140 degrees in steps of 1 degree
          yawservo.write(yaw_positionDesired);                                                    // tell servo to go to position in variable 'yaw_positionDesired'
          delay(25);                                                                              // waits 25 ms for the servo to reach the position
        }
        for (yaw_positionDesired = 140; yaw_positionDesired >= 110; yaw_positionDesired -= 1) { // goes from 140 degrees to 110 degrees
          yawservo.write(yaw_positionDesired);                                                  // tell servo to go to position in variable 'yaw_positionDesired'
          delay(25);                                                                            // waits 25 ms for the servo to reach the position
        }
        yaw_counter++;
      }
      yawservo.write(yaw_home);
      pitchservo.write(pitch_home);
      yaw_check = 1;
      active = 1;
      screen = 1;
    }
    else if ((pitch_check + yaw_check) == 2) {
      flag = 2;
    }

    else if (button1_state == HIGH && button2_state == HIGH) {
      flag = 2;
    }

    else {
      if (active == 1) {
        lcd.on();
        lcd.empty();
        lcd.print("Btn1 for Pitch\0");
        lcd.pos(1, 0);
        lcd.print("Btn2 for Yaw\0");
        active = 2;
      }
      pitch_counter = 0;
      yaw_counter = 0;
    }

  }
}

//-------------------------------------------- Passive Rehab (Phase-2) ------------------------------------------

void Passive() {
  while (passive_count <= passive_rep) {
    pitchservo.detach();
    if (passive == 1) {
      lcd.on();
      lcd.empty();
      lcd.print("Keep pushing\0");
      lcd.pos(1, 0);
      lcd.print("Btn3 for home\0");
      passive = 2;
    }

    if (digitalRead(Lswitch) == HIGH) {
      pitchservo.attach(9);
      lcd.empty();
      lcd.print("Pitch activated\0");
      pitchservo.write(pitch_home);
      delay(500);
      lcd.empty();
      passive_count++;
      pitchservo.detach();
      yawservo.write(yaw_home);
      passive = 1;
    }
    screen = 1;
  }
  pitchservo.write(pitch_home);
  yawservo.write(yaw_home);
}
