
#include <Servo.h> //Servo library

Servo servo1; //Creating servo object

//Pins button and servo are connected to
const int buttonPin = 2;
const int servo1Pin = 9;

//Preset Servo angle settings
//const int initialPos;
const int readyPos;
const int highFivePos;
const int finalPos;


//Variables relating to state of button/servo
bool highFiveOccuring = false;
bool highFiveComplete = false;
int buttonState = 0;
int pos = readyPos;



void setup() {
  servo1.attach(servo1Pin); // attaches the servo on pin 9 to servo object
  pinMode(buttonPin, INPUT); // initialize the pin for button as input

  //servo1.write(pos) tells servo to go to position 'pos' (degrees)

  servo1.write(readyPos); //servo1.write(intitialPos);
  delay(500); // waits x time for the servo to reach the position

}

void loop() {
highFive(170, 110, 170); //random arbritary values put in
delay(1000);
}



void highFive(const int readyPos, const int highFivePos, const int finalPos)
{

  highFiveOccuring == true; //to remove at some point


  while ( servo1.read() > highFivePos && highFiveComplete == false && highFiveOccuring == true)
  {
    // goes from readyPos to highFivePos in steps of 1 degree

    buttonState = digitalRead(buttonPin);
    servo1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    if (buttonState == HIGH)
    {
      highFiveComplete = true;
      pos = servo1.read();
      break;
    }

    pos -= 1;

  }


  while (servo1.read() < finalPos && highFiveOccuring == true)
  {
    servo1.write(pos);
    delay(15);
    pos += 1;

  }

  highFiveOccuring = false;

}



