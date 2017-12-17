
#include <Servo.h> //Servo library

//Creating servo objects

Servo BaseRot;
Servo RArmRot;
Servo LArmRot;
Servo RArmLift;
Servo LArmLift;
Servo HeadRot;
Servo HeadLift;

//Pin button is connected to
const int HandButton = 10;

//Preset Servo angle settings
//const int initialPos;
const int readyPos = 20;
const int highFivePos = 60;
const int finalPos = 30;


//Variables relating to state of button/servo
bool highFiveOccuring = true;
bool highFiveComplete = false;
int buttonState = 0;
int pos = readyPos;



void setup() {
  LArmLift.attach(LArmLift); // attaches the servo on pin 9 to servo object
  pinMode(HandButton, INPUT); // initialize the pin for button as input

  //LArmLift.write(pos) tells servo to go to position 'pos' (degrees)

  LArmLift.write(readyPos); //LArmLift.write(intitialPos);
  delay(500); // waits x time for the servo to reach the position

}

void loop() { //loop for testing
highFive(readyPos, highFivePos, finalPos);
delay(5000); 
highFiveOccuring = true;
}



void highFive(const int readyPos, const int highFivePos, const int finalPos)
{

  highFiveOccuring == true; //to remove at some point


  while ( LArmLift.read() > highFivePos && highFiveComplete == false && highFiveOccuring == true)
  {
    // goes from readyPos to highFivePos in steps of 1 degree

    buttonState = digitalRead(HandButton);
    LArmLift.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    if (buttonState == HIGH)
    {
      highFiveComplete = true;
      pos = LArmLift.read();
      break;
    }

    pos -= 1;

  }


  while (LArmLift.read() < finalPos && highFiveOccuring == true)
  {
    LArmLift.write(pos);
    delay(15);
    pos += 1;

  }

  highFiveOccuring = false;

}



