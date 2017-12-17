
#include <Servo.h> //Servo library

//Creating servo objects

Servo BaseRot;
Servo RArmRot;
Servo LArmRot;
Servo RArmLift;
Servo LArmLift;
Servo HeadRot;
Servo HeadLift;

//Pin button and servos are connected to
const int RArmLiftPin = 5;
const int LArmLiftPin = 6;
const int HandButton = 10;

//Preset Servo angle settings
//const int initialPos;

const int readyPos = 100;
const int finalPos = 180;
//const int happyPosLeft = 180;
//const int happyPosRight = 0;

//Variables relating to state of button/servo

int pos;



void setup() {
  RArmLift.attach(RArmLiftPin);
  LArmLift.attach(LArmLiftPin); // attaches the servo on pin 3 and 4 to servo object

  //RArmLift.write(pos) tells servo to go to position 'pos' (degrees)

  RArmLift.write(readyPos);
  LArmLift.write(readyPos);
  
  //delay(15); // waits x time for the servo to reach the position

}

void happy(){
  //can we make it squeal with a sound file? later
  for (pos = readyPos; pos <= finalPos; pos -= 5) { // goes from readyPos to finalPos
    RArmLift.write(pos);              
    LArmLift.write(pos-180);                // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void neutral(){
  for (pos = finalPos; pos >= readyPos; pos += 5) { // goes from finalPos to readyPos
    // in steps of 1 degree
    RArmLift.write(pos);              
    LArmLift.write(pos-180);                // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}


